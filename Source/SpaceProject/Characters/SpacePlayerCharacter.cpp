// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpacePlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// ASpacePlayerCharacter

ASpacePlayerCharacter::ASpacePlayerCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	bReplicates = true;
	bInGravityField = false;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a FirstPersonCameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	SetReplicates(true);
	SetReplicateMovement(true);

}

void ASpacePlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//MovementDirection = 0.f;

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			PlayerController->bAutoManageActiveCameraTarget = false;
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

// Called every frame
void ASpacePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogClass, Log, TEXT("Tick: %s"), bInGravityField ? TEXT("true") : TEXT("false"));
	if (HasAuthority() || IsLocallyControlled())
	{
		AimOffset = GetBaseAimRotation();
	}
}

void ASpacePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASpacePlayerCharacter, AimOffset);
	DOREPLIFETIME(ASpacePlayerCharacter, CameraPitch);
	DOREPLIFETIME(ASpacePlayerCharacter, CharacterCopy);
	DOREPLIFETIME(ASpacePlayerCharacter, bInGravityField);
}

//////////////////////////////////////////////////////////////////////////// Input

void ASpacePlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpacePlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpacePlayerCharacter::Look);
	}
}

void ASpacePlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASpacePlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		float ClientCameraPitch;

		// FRotator CurrentRotation = CharacterCopy->FirstPersonCameraComponent->GetRelativeRotation();
		// ClientCameraPitch = FMath::Clamp(CurrentRotation.Pitch - LookAxisVector.Y, -89.f, 89.f);
		// CharacterCopy->FirstPersonCameraComponent->SetRelativeRotation(FRotator(ClientCameraPitch, 0.0f, 0.0f));

		if (!bInGravityField)
		{
			FRotator CurrentRotation = FirstPersonCameraComponent->GetRelativeRotation();
			ClientCameraPitch = FMath::Clamp(CurrentRotation.Pitch - LookAxisVector.Y, -89.f, 89.f);
			FirstPersonCameraComponent->SetRelativeRotation(FRotator(ClientCameraPitch, 0.0f, 0.0f));
		}
		else
		{
			FRotator CurrentRotation = CharacterCopy->FirstPersonCameraComponent->GetRelativeRotation();
			ClientCameraPitch = FMath::Clamp(CurrentRotation.Pitch - LookAxisVector.Y, -89.f, 89.f);
			CharacterCopy->FirstPersonCameraComponent->SetRelativeRotation(FRotator(ClientCameraPitch, 0.0f, 0.0f));
		}
		SetCameraPitch3P(ClientCameraPitch);
	}
}

void ASpacePlayerCharacter::SetCameraPitch3P_Implementation(float ClientCameraPitch)
{
	//UE_LOG(LogClass, Log, TEXT("Mensaje de registro: %f"), ClientCameraPitch);
	CameraPitch = ClientCameraPitch;
}

bool ASpacePlayerCharacter::SetCameraPitch3P_Validate(float ClientCameraPitch)
{
    return true;
}

void ASpacePlayerCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ASpacePlayerCharacter::GetHasRifle()
{
	return bHasRifle;
}
