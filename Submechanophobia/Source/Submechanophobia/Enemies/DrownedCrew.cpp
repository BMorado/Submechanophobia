// Fill out your copyright notice in the Description page of Project Settings.


#include "DrownedCrew.h"

#include "AIController.h"

ADrownedCrew::ADrownedCrew()
{

	AttackMontageEndDelegate.BindUFunction(this, FName("AttackEnd"));
	DamagedMontageEndDelegate.BindUFunction(this, FName("DamagedEnd"));
	
	
	// Change these when we have the proper model 
	CapsuleComponent->SetRelativeScale3D(FVector(1.5f, 1.5f, 2.0f));
	CapsuleComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	CapsuleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	HealthComponent->SetMaxHealth(50.0f);
	HealthComponent->SetHealth(HealthComponent->GetMaxHealth());
	
	MovementComponent->MaxSpeed = 400;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/Boss_assets/crewmate1/crewmate1_GEO.crewmate1_GEO"));
	if (enemyAsset.Succeeded())
	{
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		
		enemyMesh->SetWorldScale3D(FVector(0.6f,0.6f,0.4f));
		enemyMesh->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
		enemyMesh->SetRelativeLocation(FVector(0.0f, 15.0f, -50.0f));
		
	}
	//SetUp animations 
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("/Game/AI/DrownedCrewAI/Anims/ABP_DrowndedCrew.ABP_DrowndedCrew_C"));
	if (AnimBlueprint.Succeeded())
	{
		enemyMesh->SetAnimInstanceClass(AnimBlueprint.Class);
	}
	UClass* AIContollerClass = LoadClass<AAIController>(nullptr, TEXT("/Game/AI/DrownedCrewAI/BP_DrownedCrewAIController.BP_DrownedCrewAIController_C"));
	
	if (AIContollerClass != nullptr)
	{
		AIControllerClass = AIContollerClass;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		bUseControllerRotationYaw = true;
	}

	attackMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Boss_assets/crewmate1/Zombie_Attack_03_Montage.Zombie_Attack_03_Montage"));
	damagedMontage =  LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Boss_assets/crewmate1/Zombie_Damage_Montage.Zombie_Damage_Montage"));
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
	FVector start = enemyMesh->GetSocketLocation("PC_HeadSocket");
	FVector end = enemyMesh->GetSocketLocation("PC_HeadSocket");
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult hits;
	UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),start,end,30.0f,UEngineTypes::ConvertToTraceType(ECC_Camera)
		,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,hits,true);
	
	UGameplayStatics::ApplyDamage(hits.GetActor(),damage,nullptr,this,nullptr);
}

void ADrownedCrew::PlayAttackAnim()
{
	Super::PlayAttackAnim();
	if (UAnimInstance* AnimInstance = enemyMesh->GetAnimInstance())
	{
		AnimInstance->Montage_Play(attackMontage);
		Attack();
		//GetWorld()->GetTimerManager().SetTimer(timer,this,&ADrownedCrew::Attack,0.05f,true);
		AnimInstance->Montage_SetEndDelegate(AttackMontageEndDelegate, attackMontage);
	}
}

/*float ADrownedCrew::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	HealthComponent->TakeDamage(DamageAmount);
	if (HealthComponent->GetCurrentHealth() >= 0.0f)
	{
		if (isDamagable == true)
		{
			UAnimInstance* AnimInstance = enemyMesh->GetAnimInstance();
			AnimInstance->Montage_Play(damagedMontage);
			AnimInstance->Montage_SetEndDelegate(DamagedMontageEndDelegate, damagedMontage);
			MovementComponent->MaxSpeed = 0;
			isDamagable = false;
		}
		
	}
	if (HealthComponent->GetCurrentHealth() <= 0.0f)
	{
		this->Destroy();
	}
	return DamageAmount;
}*/

void ADrownedCrew::AttackEnd()
{
	OnAttackEnd.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(timer);
}

void ADrownedCrew::DamagedEnd()
{
	isDamagable = true;
	MovementComponent->MaxSpeed = 400.0f;
}
