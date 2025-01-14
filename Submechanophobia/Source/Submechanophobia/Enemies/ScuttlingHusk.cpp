// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "ScuttlingHusk.h"


AScuttlingHusk::AScuttlingHusk()
{
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	enemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Enemy Mesh"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->Initialize(100);

	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/Boss_assets/Scuttling_Husk/scuttling_husk_exp_nov13.scuttling_husk_exp_nov13"));
	if (enemyAsset.Succeeded())
	{
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		enemyMesh->SetupAttachment(RootComponent);
		enemyMesh->SetRelativeLocation(FVector::ZeroVector);
		enemyMesh->SetRelativeScale3D(FVector(30.0));
		enemyMesh->SetSimulatePhysics(true);
		
		enemyMesh->SetEnableGravity(true); 
		enemyMesh->SetCollisionProfileName("Pawn");
		enemyMesh->BodyInstance.bLockXRotation = true;
		enemyMesh->BodyInstance.bLockYRotation = true;
		enemyMesh->BodyInstance.bLockZRotation = true;

	
	}

}


void AScuttlingHusk::BeginPlay()
{
	Super::BeginPlay();
	//DrawDebugCapsule(GetWorld(), collider->GetComponentLocation(), collider->GetUnscaledCapsuleHalfHeight(), collider->GetUnscaledCapsuleHalfHeight(),collider->GetRelativeRotation().Quaternion(), FColor::Green, true, -1.0f, 0, 2.0f);
}

void AScuttlingHusk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
