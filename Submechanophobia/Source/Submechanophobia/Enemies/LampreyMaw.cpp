// Fill out your copyright notice in the Description page of Project Settings.


#include "LampreyMaw.h"

ALampreyMaw::ALampreyMaw()
{
	// Change these when we have the proper model 
	CapsuleComponent->SetRelativeScale3D(FVector(3.066639f, 1.0, 2.818257));
	CapsuleComponent->SetRelativeRotation(FRotator(90.0f, 90.0f, 180.0f));
	CapsuleComponent->SetRelativeLocation(FVector(14.1915f, 0.0f, 58.972f));

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/Boss_assets/Lamprey_Maw/LaMa_exp_feb3.LaMa_exp_feb3"));
	//if (enemyAsset.Succeeded())
	//{
	//	// Set the mesh object, scale and location 
	//	enemyMesh->SetSkeletalMesh(enemyAsset.Object);
	//	enemyMesh->SetWorldScale3D(FVector(10.0f,30.0f,10.0f));
	//	enemyMesh->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
	//	enemyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));

	//}
	
}

void ALampreyMaw::BeginPlay()
{
	Super::BeginPlay();
}

void ALampreyMaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALampreyMaw::Attack()
{
	Super::Attack();
}
