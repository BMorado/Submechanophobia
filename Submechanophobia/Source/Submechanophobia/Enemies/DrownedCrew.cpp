// Fill out your copyright notice in the Description page of Project Settings.


#include "DrownedCrew.h"

ADrownedCrew::ADrownedCrew()
{
	// Change these when we have the proper model 
	CapsuleComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	CapsuleComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	CapsuleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/Boss_assets/crewmate1/crewmate1_GEO.crewmate1_GEO"));
	if (enemyAsset.Succeeded())
	{
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		
		enemyMesh->SetWorldScale3D(FVector(1.0f,1.0f,1.0f));
		enemyMesh->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
		enemyMesh->SetRelativeLocation(FVector(1.0f, 1.0f, 1.0f));
		
	}
	//SetUp animations 
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("/Game/AI/DrownedCrewAI/Anims/ABP_DrowndedCrew.ABP_DrowndedCrew_C"));
	if (AnimBlueprint.Succeeded())
	{
		enemyMesh->SetAnimInstanceClass(AnimBlueprint.Class);
	}
	
}

void ADrownedCrew::BeginPlay()
{
	Super::BeginPlay();
}

void ADrownedCrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADrownedCrew::Attack()
{
	Super::Attack();
}
