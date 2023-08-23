// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePickableCopy3P.h"

// Sets default values
ASpacePickableCopy3P::ASpacePickableCopy3P()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ASpacePickableCopy3P::BeginPlay()
{
	Super::BeginPlay();
}

void ASpacePickableCopy3P::SetSkeletalMeshAsRoot(USkeletalMeshComponent* SkeletalMeshComponent)
{
	WeaponMesh = SkeletalMeshComponent;
	SetRootComponent(SkeletalMeshComponent);
	SkeletalMeshComponent->SetOnlyOwnerSee(false);
	SkeletalMeshComponent->SetOwnerNoSee(true);
}

