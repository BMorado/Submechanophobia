// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	enemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = enemyMesh;
	
	// Optionally assign a default skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(TEXT("/Game/Boss_assets/Scuttling_Husk/scuttling_husk_exp_nov13.scuttling_husk_exp_nov13"));
	if (DefaultMesh.Succeeded())
	{
		enemyMesh->SetSkeletalMesh(DefaultMesh.Object);
		enemyMesh->SetupAttachment(RootComponent);
		enemyMesh->SetRelativeRotation(FRotator(0, 180.0f, 0));
		enemyMesh->SetRelativeLocation(FVector::ZeroVector);
		enemyMesh->SetRelativeScale3D(FVector(30.0f));

	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

