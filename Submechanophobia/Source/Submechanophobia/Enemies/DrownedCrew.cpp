// Fill out your copyright notice in the Description page of Project Settings.


#include "DrownedCrew.h"

ADrownedCrew::ADrownedCrew()
{
	// Change these when we have the proper model 
	CapsuleComponent->SetRelativeScale3D(FVector(3.066639f, 3.066639, 2.818257));
	CapsuleComponent->SetRelativeRotation(FRotator(90.0f, 90.0f, 180.0f));
	CapsuleComponent->SetRelativeLocation(FVector(14.1915f, 0.0f, 58.972f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/TEST/Enemy/Test_Enemy_Stuff/The_Boss.The_Boss"));
	if (enemyAsset.Succeeded())
	{
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		enemyMesh->SetWorldScale3D(FVector(0.560529,0.560529,0.417699));
		enemyMesh->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
		enemyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -41.7699f));

		
		
		
		/*// Enable Physics 
		enemyMesh->SetSimulatePhysics(true);
		enemyMesh->SetCollisionProfileName("Pawn");

		// Stops you from being able to flip the Enemy 
		enemyMesh->BodyInstance.bLockXRotation = true;
		enemyMesh->BodyInstance.bLockYRotation = true;
		enemyMesh->BodyInstance.bLockZRotation = true;*/
	}
	//SetUp animations 
	//static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("/Game/TEST/Enemy/Test_Enemy_Stuff/Anims/Enemy_ABP.Enemy_ABP"));
	//enemyMesh->AnimClass = AnimBlueprint.Object->GeneratedClass;
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
