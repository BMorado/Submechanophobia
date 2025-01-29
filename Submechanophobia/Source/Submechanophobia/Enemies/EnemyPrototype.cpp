// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPrototype.h"

#include <iostream>
#include <ostream>

#include "EnemyAIController.h"
#include <string>
#include "AIController.h"



AEnemyPrototype::AEnemyPrototype()
{
	
	bUseControllerRotationYaw = true;

	
		AIControllerClass = AEnemyAIController::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/TEST/Enemy/Test_Enemy_Stuff/The_Boss.The_Boss"));
	CapsuleComponent->SetRelativeScale3D(FVector(1.784029,1.784029,2.394067));
	if (enemyAsset.Succeeded())
	{
		
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		enemyMesh->SetupAttachment(CapsuleComponent);
		enemyMesh->SetWorldScale3D(FVector(0.560529,0.560529,0.417699));
		enemyMesh->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
		enemyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -41.7699f));

		//SetUp animations 
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("/Game/TEST/Enemy/Test_Enemy_Stuff/Anims/Enemy_ABP.Enemy_ABP"));
		if (AnimBlueprint.Succeeded())
		{
			enemyMesh->SetAnimInstanceClass(AnimBlueprint.Object->GeneratedClass);
			UE_LOG(LogTemp, Log, TEXT("Found AnimBlueprint"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find AnimBlueprint"));
		}
		
		/*// Enable Physics 
		enemyMesh->SetSimulatePhysics(true);
		enemyMesh->SetCollisionProfileName("Pawn");

		// Stops you from being able to flip the Enemy 
		enemyMesh->BodyInstance.bLockXRotation = true;
		enemyMesh->BodyInstance.bLockYRotation = true;
		enemyMesh->BodyInstance.bLockZRotation = true;*/
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyPrototype::BeginPlay()
{
	Super::BeginPlay();
	SetDamage(15);
}

void AEnemyPrototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Attack();
}

void AEnemyPrototype::Attack(){
	Super::Attack();
	FVector start = enemyMesh->GetBoneLocation("LeftHandMiddle1");
	FVector end = enemyMesh->GetBoneLocation("LeftHandMiddle1");
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult hits;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),start,end,30.0f,UEngineTypes::ConvertToTraceType(ECC_Camera)
		,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,hits,true);
	
	
	
	UGameplayStatics::ApplyDamage(hits.GetActor(),GetDamage(),nullptr,this,nullptr);
}
