// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "SpaceShipVehicle.generated.h"

UCLASS()
class SPACEPROJECT_API ASpaceShipVehicle : public APawn
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	ASpaceShipVehicle();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Ship Mesh */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	UStaticMeshComponent* ShipMesh;

	/** Pilot Chair Mesh */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	UStaticMeshComponent* PilotChair;

	/** Gravity box */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	UBoxComponent* GravityBox;

	/** Center of mass for movement */
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	UArrowComponent* CenterOfMass;

};
