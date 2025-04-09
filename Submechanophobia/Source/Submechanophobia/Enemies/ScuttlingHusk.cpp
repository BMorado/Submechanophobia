



#include "ScuttlingHusk.h"
#include "ScuttlingHuskAIC.h"


// make health a little random 

AScuttlingHusk::AScuttlingHusk()
{
	AttackMontageEndDelegate.BindUFunction(this, FName("AttackEnd"));
	DamagedMontageEndDelegate.BindUFunction(this, FName("DamagedEnd"));
	
	HealthComponent->SetMaxHealth(50.0f);
	HealthComponent->SetHealth(HealthComponent->GetMaxHealth());
	
	// Set Max movement speed
	MovementComponent->MaxSpeed = 400;
	
	// Set Up capsule Size
	CapsuleComponent->SetRelativeScale3D(FVector(3.066639f, 3.066639, 2.818257));
	CapsuleComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	CapsuleComponent->SetRelativeLocation(FVector(14.1915f, 0.0f, 58.972f));
	
	// Set up skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> enemyAsset(TEXT("/Game/Boss_assets/Scuttling_Husk/scuttling_husk_exp_feb2.scuttling_husk_exp_feb2"));
	if (enemyAsset.Succeeded())
	{
		// Set the mesh object, scale and location 
		enemyMesh->SetSkeletalMesh(enemyAsset.Object);
		enemyMesh->SetupAttachment(CapsuleComponent);
		enemyMesh->SetWorldScale3D(FVector(9.0));
		enemyMesh->SetRelativeRotation(FRotator(-90.0, -90.0f, 0.0f));
		enemyMesh->SetRelativeLocation(FVector(0.0f, 23.0f, 0.0f));
		enemyMesh->SetIsReplicated(true);
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
	}
	// Load Animations
	attackMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Boss_assets/Scuttling_Husk/ScuttlingHusk_Attack_Montage.ScuttlingHusk_Attack_Montage"));
	damagedMontage =  LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Boss_assets/Scuttling_Husk/crab_damaged_Montage.crab_damaged_Montage"));

}

void AScuttlingHusk::BeginPlay()
{
	Super::BeginPlay();
}

void AScuttlingHusk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AScuttlingHusk::AttackEnd()
{
	OnAttackEnd.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(timer);
}

void AScuttlingHusk::DamagedEnd()
{
		isDamagable = true;
		MovementComponent->MaxSpeed = 400.0f;
		
}



void AScuttlingHusk::Attack() 
{
	Super::Attack();
	FVector start = enemyMesh->GetSocketLocation("RFH_Attack_Socket");
	FVector end = enemyMesh->GetSocketLocation("RFH_Attack_Socket");
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult hits;
	UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),start,end,30.0f,UEngineTypes::ConvertToTraceType(ECC_Camera)
		,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,hits,true);
	
	UGameplayStatics::ApplyDamage(hits.GetActor(),damage,nullptr,this,nullptr);
	
}


void AScuttlingHusk::PlayAttackAnim()
{
	Super::PlayAttackAnim();
	/*if (UAnimInstance* AnimInstance = enemyMesh->GetAnimInstance())
	{
		AnimInstance->Montage_Play(attackMontage);
		GetWorld()->GetTimerManager().SetTimer(timer,this,&AScuttlingHusk::Attack,0.05f,true);
		AnimInstance->Montage_SetEndDelegate(AttackMontageEndDelegate, attackMontage);
	}*/
}

/*float AScuttlingHusk::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
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

void AScuttlingHusk::GetPatrolRoute(AEnemySpline*& PatrolRoute)
{
	IEnemyInter::GetPatrolRoute(PatrolRoute);
	PatrolRoute = PatrolRoutePath;
}


