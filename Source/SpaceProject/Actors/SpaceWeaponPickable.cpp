// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceWeaponPickable.h"
#include "Kismet/GameplayStatics.h"
#include "SpacePickableCopy3P.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASpaceWeaponPickable::ASpaceWeaponPickable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAlreadyCopied = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Capsule = CreateDefaultSubobject<USphereComponent>(TEXT("Capsule"));

	SetRootComponent(Capsule);
	WeaponMesh->SetupAttachment(RootComponent);

	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ASpaceWeaponPickable::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ASpaceWeaponPickable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceWeaponPickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASpaceWeaponPickable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<ASpacePlayerCharacter>(OtherActor);
	if(Character != nullptr && !bAlreadyCopied)
	{
		bAlreadyCopied = true;
		ASpacePickableCopy3P* ThirdPersonCopy = GetWorld()->SpawnActor<ASpacePickableCopy3P>(ASpacePickableCopy3P::StaticClass(), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
		AttachWeaponMesh(this);

		if (!HasAuthority())
		{
			AttachWeaponMesh3P(ThirdPersonCopy);
		}

		// switch bHasRifle so the animation blueprint can switch to another animation set
		Character->SetHasRifle(true);

		// Set up action bindings
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
				Subsystem->AddMappingContext(FireMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// Fire
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ASpaceWeaponPickable::Fire);
			}
		}
	}
}

void ASpaceWeaponPickable::AttachWeaponMesh(ASpaceWeaponPickable* WeaponPickable)
{
	if (Character == nullptr)
	{
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	WeaponPickable->AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	WeaponPickable->SetOwner(Character);
	WeaponPickable->WeaponMesh->SetOnlyOwnerSee(true);
}

void ASpaceWeaponPickable::AttachWeaponMesh3P(ASpacePickableCopy3P* WeaponPickable)
{
	if (Character == nullptr)
	{
		return;
	}

	bAlreadyCopied = true;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	WeaponPickable->AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	WeaponPickable->SetOwner(Character);
	WeaponPickable->SetReplicates(true);
	USkeletalMeshComponent* RootComponentRef = Cast<USkeletalMeshComponent>(WeaponPickable->GetRootComponent());
	RootComponentRef->SetSkeletalMeshAsset(WeaponMesh->GetSkeletalMeshAsset());
	RootComponentRef->SetOnlyOwnerSee(false);
	RootComponentRef->SetOwnerNoSee(true);
}

void ASpaceWeaponPickable::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// // Try and fire a projectile
	// if (ProjectileClass != nullptr)
	// {
	// 	UWorld* const World = GetWorld();
	// 	if (World != nullptr)
	// 	{
	// 		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	// 		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// 		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	// 		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
	// 		//Set Spawn Collision Handling Override
	// 		FActorSpawnParameters ActorSpawnParams;
	// 		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	// 		// Spawn the projectile at the muzzle
	// 		World->SpawnActor<ASpaceProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	// 	}
	// }
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

