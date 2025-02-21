// Fill out your copyright notice in the Description page of Project Settings.


#include "ScuttlingHusk.h"
#include "DrawDebugHelpers.h"
#include "EnemyAIController.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"


// TODO: scale the capsule to the correct size, fobjetc find the mesh and check if mesh is valid, set enemy mesh to found mesh 
AScuttlingHusk::AScuttlingHusk()
{

	CapsuleComponent->SetRelativeScale3D(FVector(3.066639f, 3.066639, 2.818257));
	CapsuleComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	CapsuleComponent->SetRelativeLocation(FVector(14.1915f, 0.0f, 58.972f));
	
	// Set up skel mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/Boss_assets/Scuttling_Husk/scuttling_husk_exp_feb2.scuttling_husk_exp_feb2"));
	static ConstructorHelpers::FClassFinder<AEnemyAIController> AIClass(TEXT("/Game/TEST/Enemy/Test_Enemy_Stuff/MyEnemyAIControllerBP.MyEnemyAIControllerBP"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("/Game/AI/ScuttlingHuskAI/Anims/Crab_ABP.Crab_ABP"));
	if (enemyAsset.Succeeded())
	{
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		enemyMesh->SetupAttachment(CapsuleComponent);
		enemyMesh->SetWorldScale3D(FVector(10.0));
		enemyMesh->SetRelativeRotation(FRotator(-90.0, -90.0f, 0.0f));
		enemyMesh->SetRelativeLocation(FVector(0.0f, 23.0f, 0.0f));
		enemyMesh->AnimClass = AnimBlueprint.Object->GeneratedClass;
		
		

		// Stops you from being able to flip the Enemy 
		/*enemyMesh->BodyInstance.bLockXRotation = true;
		enemyMesh->BodyInstance.bLockYRotation = true;
		enemyMesh->BodyInstance.bLockZRotation = true;*/

		bUseControllerRotationYaw = true;
		//bUseControllerRotationPitch = true;
		//bUseControllerRotationRoll = true;
	}
	else{
		UE_LOG(LogTemp, Error, TEXT("enemyMesh or enemyMeshAsset is null! Check if it was created properly."));
	}
	
		AIControllerClass = AIClass.Class;
	
	//AIControllerClass = AEnemyAIController::StaticClass();
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


void AScuttlingHusk::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(timer,this,&AScuttlingHusk::Attack,0.25f,true);
	
	
}

void AScuttlingHusk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
