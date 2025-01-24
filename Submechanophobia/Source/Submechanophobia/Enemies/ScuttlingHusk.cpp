// Fill out your copyright notice in the Description page of Project Settings.


#include "ScuttlingHusk.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


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
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		enemyMesh->SetupAttachment(RootComponent);
		enemyMesh->SetRelativeScale3D(FVector(30.0));
		enemyMesh->SetRelativeRotation(FRotator(0, -90, 0));
		enemyMesh->SetWorldLocation(FVector::ZeroVector, false, nullptr, ETeleportType::TeleportPhysics);

		// Enable Physics 
		enemyMesh->SetSimulatePhysics(true);
		enemyMesh->SetCollisionProfileName("Pawn");

		// Stops you from being able to flip the Enemy 
		enemyMesh->BodyInstance.bLockXRotation = true;
		enemyMesh->BodyInstance.bLockYRotation = true;
		enemyMesh->BodyInstance.bLockZRotation = true;
	}
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
}


void AScuttlingHusk::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->GetTimerManager().SetTimer(Timer,this,AScuttlingHusk::Attack(),5.0f,true);
	//DrawDebugCapsule(GetWorld(), collider->GetComponentLocation(), collider->GetUnscaledCapsuleHalfHeight(), collider->GetUnscaledCapsuleHalfHeight(),collider->GetRelativeRotation().Quaternion(), FColor::Green, true, -1.0f, 0, 2.0f);
}

void AScuttlingHusk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Attack();
}

void AScuttlingHusk::Attack() 
{
	
	Super::Attack();
	FVector start = enemyMesh->GetBoneLocation("claw2_L");
	FVector end = enemyMesh->GetBoneLocation("claw2_L");
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult hits;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),start,end,30.0f,UEngineTypes::ConvertToTraceType(ECC_Camera)
		,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,hits,true);
	
		UGameplayStatics::ApplyDamage(hits.GetActor(),damage,nullptr,this,nullptr);

	
}
