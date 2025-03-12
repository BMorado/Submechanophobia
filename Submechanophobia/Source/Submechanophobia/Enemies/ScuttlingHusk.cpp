// Fill out your copyright notice in the Description page of Project Settings.


#include "ScuttlingHusk.h"
#include "ScuttlingHuskAIC.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"


// TODO: scale the capsule to the correct size, fobjetc find the mesh and check if mesh is valid, set enemy mesh to found mesh 
AScuttlingHusk::AScuttlingHusk()
{

	MontageEndDelegate.BindUFunction(this, FName("AttackEnd"));
	
	CapsuleComponent->SetRelativeScale3D(FVector(3.066639f, 3.066639, 2.818257));
	CapsuleComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	CapsuleComponent->SetRelativeLocation(FVector(14.1915f, 0.0f, 58.972f));
	//CapsuleComponent->SetSimulatePhysics(true);
	
	// Set up skel mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/Boss_assets/Scuttling_Husk/scuttling_husk_exp_feb2.scuttling_husk_exp_feb2"));
	if (enemyAsset.Succeeded())
	{
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		enemyMesh->SetupAttachment(CapsuleComponent);
		enemyMesh->SetWorldScale3D(FVector(9.0));
		enemyMesh->SetRelativeRotation(FRotator(-90.0, -90.0f, 0.0f));
		enemyMesh->SetRelativeLocation(FVector(0.0f, 23.0f, 0.0f));
		
		// Stops you from being able to flip the Enemy 
		//enemyMesh->BodyInstance.bLockXRotation = true;
		//enemyMesh->BodyInstance.bLockYRotation = true;
		//enemyMesh->BodyInstance.bLockZRotation = true;
	}

	// Load ABP Class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("/Game/AI/ScuttlingHuskAI/Anims/Crab_ABP.Crab_ABP_C"));
	if (AnimBlueprint.Succeeded())
	{
		enemyMesh->SetAnimInstanceClass(AnimBlueprint.Class);
	}

	// Load AI Controller class 
	UClass* AIContollerClass = LoadClass<AAIController>(nullptr, TEXT("/Game/AI/ScuttlingHuskAI/AIC_ScuttlingHusk.AIC_ScuttlingHusk_C"));
	if (AIContollerClass != nullptr)
	{
		AIControllerClass = AIContollerClass;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		bUseControllerRotationYaw = true;
		//bUseControllerRotationPitch = true;
		//bUseControllerRotationRoll = true;
	}
	attackMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Boss_assets/Scuttling_Husk/ScuttlingHusk_Attack_Montage.ScuttlingHusk_Attack_Montage"));
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage(TEXT("/Game/Boss_assets/Scuttling_Husk/Crab_Attack_Montage.Crab_Attack_Montage"));
	if (AttackMontage.Succeeded())
	{
		attackMontage = AttackMontage.Object;
	}*/
	
}

void AScuttlingHusk::BeginPlay()
{
	Super::BeginPlay();
	
	//GetWorld()->GetTimerManager().SetTimer(timer,this,&AScuttlingHusk::Attack,0.25f,true);
	
	
}

void AScuttlingHusk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScuttlingHusk::AttackEnd()
{
	OnAttackEnd.Broadcast();
}

void AScuttlingHusk::Attack() 
{
	Super::Attack();

		if (UAnimInstance* AnimInstance = enemyMesh->GetAnimInstance())
		{
			AnimInstance->Montage_Play(attackMontage);
			AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, attackMontage);
			
		}
	UE_LOG(LogTemp, Log, TEXT("Enemy Mesh is not assigned!"));

	FVector start = enemyMesh->GetSocketLocation("RFH_Attack_Socket");
	FVector end = enemyMesh->GetSocketLocation("RFH_Attack_Socket");
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult hits;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),start,end,30.0f,UEngineTypes::ConvertToTraceType(ECC_Camera)
		,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,hits,true);
	
	UGameplayStatics::ApplyDamage(hits.GetActor(),damage,nullptr,this,nullptr);

	
}
