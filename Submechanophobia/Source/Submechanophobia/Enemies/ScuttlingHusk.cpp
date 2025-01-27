// Fill out your copyright notice in the Description page of Project Settings.


#include "ScuttlingHusk.h"
#include "DrawDebugHelpers.h"




AScuttlingHusk::AScuttlingHusk()
{

	CapsuleComponent->SetRelativeScale3D(FVector(3.066639f, 3.066639, 2.818257));
	CapsuleComponent->SetRelativeRotation(FRotator(90.0f, 90.0f, 180.0f));
	CapsuleComponent->SetRelativeLocation(FVector(14.1915f, 0.0f, 58.972f));
	
	// Set up skel mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/Boss_assets/Scuttling_Husk/scuttling_husk_exp_nov13.scuttling_husk_exp_nov13"));
	if (enemyAsset.Succeeded())
	{
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		enemyMesh->SetupAttachment(CapsuleComponent);
		enemyMesh->SetWorldScale3D(FVector(10.0));
		enemyMesh->SetWorldRotation(FRotator(90, 90, -90));
		enemyMesh->SetRelativeLocation(FVector(-15.0f, 3.0f, 0.0f));
		
		
		/*// Enable Physics 
		enemyMesh->SetSimulatePhysics(true);
		enemyMesh->SetCollisionProfileName("Pawn");

		// Stops you from being able to flip the Enemy 
		enemyMesh->BodyInstance.bLockXRotation = true;
		enemyMesh->BodyInstance.bLockYRotation = true;
		enemyMesh->BodyInstance.bLockZRotation = true;*/
	}
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
	
	/*Super::Attack();
	FVector start = enemyMesh->GetBoneLocation("claw2_L");
	FVector end = enemyMesh->GetBoneLocation("claw2_L");
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult hits;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),start,end,30.0f,UEngineTypes::ConvertToTraceType(ECC_Camera)
		,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,hits,true);
	
		UGameplayStatics::ApplyDamage(hits.GetActor(),damage,nullptr,this,nullptr);
		*/

	
}
