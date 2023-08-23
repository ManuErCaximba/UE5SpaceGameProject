// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipVehicle.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASpaceShipVehicle::ASpaceShipVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	PilotChair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PilotChair"));
	GravityBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GravityBox"));
	CenterOfMass = CreateDefaultSubobject<UArrowComponent>(TEXT("CenterOfMass"));

	FName SocketName = FName(TEXT("SocketName"));
	SetRootComponent(ShipMesh);
	GravityBox->SetupAttachment(RootComponent, SocketName);
	CenterOfMass->SetupAttachment(RootComponent, SocketName);
	PilotChair->SetupAttachment(RootComponent, SocketName);
}

// Called when the game starts or when spawned
void ASpaceShipVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceShipVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpaceShipVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASpaceShipVehicle, CenterOfMass);
}

