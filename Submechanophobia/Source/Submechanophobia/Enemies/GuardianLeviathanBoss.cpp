// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianLeviathanBoss.h"
#include "Submechanophobia/Player/APlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


AGuardianLeviathanBoss::AGuardianLeviathanBoss()
{
    // === Create and attach the skeletal mesh component ===
    enemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
    enemyMesh->SetupAttachment(RootComponent); // Or CapsuleComponent if that's your root

    // === Animation montage end delegate setup ===
    MontageEndDelegate.BindUFunction(this, FName("OnMontageEnded"));

    HealthComponent->SetMaxHealth(100.0f);
    HealthComponent->SetHealth(HealthComponent->GetMaxHealth());


    //Mesh and Anim BP assignment should happen in the Blueprint for flexibility.
    
}

void AGuardianLeviathanBoss::BeginPlay()
{
    Super::BeginPlay();
    
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsInt(FName("CurrentStage"), CurrentStage); // default = 1
        }
    }
}

void AGuardianLeviathanBoss::EnterNextStage()
{
    CurrentStage++;
}

void AGuardianLeviathanBoss::FireAttack()
{
    /*if (!FireMontage) return;

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;

    UE_LOG(LogTemp, Log, TEXT("Boss performing Fire Breath"));
    Anim->Montage_SetEndDelegate(MontageEndDelegate, FireMontage);
    Anim->Montage_Play(FireMontage);*/
    USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh) return;

    UAnimInstance* Anim = Mesh->GetAnimInstance();
    if (!Anim || !FireMontage) return;

    Anim->Montage_Play(FireMontage);

    UParticleSystemComponent* FireVFX = FindComponentByClass<UParticleSystemComponent>();
    FireVFX->Activate(true);


    //USED FOR TESTING
    HealthComponent->TakeDamage(10.0f);
    float CurrentHealth = HealthComponent->GetCurrentHealth();

    UE_LOG(LogTemp, Warning, TEXT("Boss current health (after self-damage): %.2f"), CurrentHealth);

    OnBossDamaged(CurrentHealth); // Will trigger stage logic if needed
    //END OF TESTING


    // Start damage over time
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGuardianLeviathanBoss::ApplyFireDamage, 0.5f, true, 0.0f);
    GetWorldTimerManager().SetTimer(StopFireHandle, this, &AGuardianLeviathanBoss::StopFireBreath, 5.0f, false);
}

void AGuardianLeviathanBoss::ApplyFireDamage()
{
    TArray<AActor*> Players;
    UCapsuleComponent* fireHitbox = FindComponentByClass<UCapsuleComponent>();
    

    fireHitbox->GetOverlappingActors(Players, AAPlayerCharacter::StaticClass());

    for (AActor* Player : Players)
    {
        // Apply damage
        if (AAPlayerCharacter* Target = Cast<AAPlayerCharacter>(Player))
        {
            UGameplayStatics::ApplyDamage(Target, 10.0f, GetController(), this, nullptr);
        }
    }
}

void AGuardianLeviathanBoss::StopFireBreath()
{
    UParticleSystemComponent* FireVFX = FindComponentByClass<UParticleSystemComponent>();
    FireVFX->Deactivate();
    GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AGuardianLeviathanBoss::ScreechAttack()
{
    if (!ScreechMontage) return;

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;

    UE_LOG(LogTemp, Log, TEXT("Boss performing Screech"));
    Anim->Montage_SetEndDelegate(MontageEndDelegate, ScreechMontage);
    Anim->Montage_Play(ScreechMontage);

    // Damage only — stun logic omitted as requested
    TArray<AActor*> Players;
    GetOverlappingActors(Players, AAPlayerCharacter::StaticClass());

    for (AActor* Player : Players)
    {
        if (AAPlayerCharacter* Target = Cast<AAPlayerCharacter>(Player))
        {
            UGameplayStatics::ApplyDamage(Target, 10.0f, GetController(), this, nullptr);
        }
    }
}

void AGuardianLeviathanBoss::LungeAttack()
{
    if (!LungeMontage) return;

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;

    UE_LOG(LogTemp, Log, TEXT("Boss performing Lunge"));
    Anim->Montage_SetEndDelegate(MontageEndDelegate, LungeMontage);
    Anim->Montage_Play(LungeMontage);

    // Movement logic can be expanded later
}

void AGuardianLeviathanBoss::TransitionOut()
{
    /*if (!TransitionMontage) return;

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;*/
    USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh) return;

    UAnimInstance* Anim = Mesh->GetAnimInstance();
    if (!Anim || !TransitionMontage) return;

    Anim->Montage_Play(TransitionMontage);

    /*UE_LOG(LogTemp, Log, TEXT("Boss transitioning out"));
    Anim->Montage_SetEndDelegate(MontageEndDelegate, TransitionMontage);
    Anim->Montage_Play(TransitionMontage);*/
}

void AGuardianLeviathanBoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("Montage ended: %s"), *Montage->GetName());
}

void AGuardianLeviathanBoss::MoveToNewHole()
{
    if (HoleTransforms.Num() == 0) return;

    int32 NewIndexSpot;
    do {
        NewIndexSpot = FMath::RandRange(0, HoleTransforms.Num() - 1);
    } while (NewIndexSpot == LastHoleIndex && HoleTransforms.Num() > 1);

    LastHoleIndex = NewIndexSpot;

    const FSpawnHole& Target = HoleTransforms[NewIndexSpot];

    // Move to new location and rotation
    SetActorLocationAndRotation(Target.Location, Target.Rotation);
}

void AGuardianLeviathanBoss::OnBossDamaged(float CurrentHealth)
{
    /*AAIController* AIController = Cast<AAIController>(GetController());
    if (!AIController) return;

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp) return;*/


    if (CurrentStage == 1 && CurrentHealth <= 66.f)
    {
        CurrentStage = 2;
        UE_LOG(LogTemp, Warning, TEXT("Stage 2 started!"));

        // === Spawn second serpent ===
        if (UWorld* World = GetWorld())
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            // Pick a new hole that is not the current one
            int32 SecondIndex = -1;
            do {
                SecondIndex = FMath::RandRange(0, HoleTransforms.Num() - 1);
            } while (SecondIndex == LastHoleIndex && HoleTransforms.Num() > 1);

            const FSpawnHole& SecondHole = HoleTransforms[SecondIndex];

            AGuardianLeviathanBoss* SecondSerpent = World->SpawnActor<AGuardianLeviathanBoss>(
                GetClass(), // Spawning another of the same class
                SecondHole.Location,
                SecondHole.Rotation,
                SpawnParams
            );

            if (SecondSerpent)
            {
                SecondSerpent->HoleTransforms = this->HoleTransforms; // Give them the same hole list
                UE_LOG(LogTemp, Warning, TEXT("Second serpent spawned at hole %d"), SecondIndex);
            }
        }

    }
    else if (CurrentStage == 2 && CurrentHealth <= 33.f)
    {
        CurrentStage = 3;
        UE_LOG(LogTemp, Warning, TEXT("Stage 3 started!"));
    }

    //Update the BB for CurrentStage
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsInt(FName("CurrentStage"), CurrentStage);
        }
    }

}

float AGuardianLeviathanBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    if (HealthComponent)
    {
        HealthComponent->TakeDamage(DamageAmount);

        float CurrentHealth = HealthComponent->GetCurrentHealth();

        // Call stage logic
        OnBossDamaged(CurrentHealth);

        // Optional death check
        if (CurrentHealth <= 0.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Boss has been defeated!"));
            Destroy();  // Or trigger final phase/cinematic/etc.
        }
    }

    return DamageAmount;
}