// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpacePickableCopy3P.generated.h"

UCLASS()
class SPACEPROJECT_API ASpacePickableCopy3P : public AActor
{
	GENERATED_BODY()

	/** Weapon Mesh */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* WeaponMesh;
	
public:	
	// Sets default values for this actor's properties
	ASpacePickableCopy3P();

	void SetSkeletalMeshAsRoot(USkeletalMeshComponent* SkeletalMeshComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
