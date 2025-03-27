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

    // === Optional AI setup (handled in Blueprint now) ===
    // AIControllerClass = AGuardianLeviathanAIC::StaticClass();
    // AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    // bUseControllerRotationYaw = true;

    // === Mesh and AnimBP assignment are now handled in Blueprint ===
    
}

void AGuardianLeviathanBoss::BeginPlay()
{
    Super::BeginPlay();
    /*BossAIC = Cast<AGuardianLeviathanAIC>(GetController());*/
    UE_LOG(LogTemp, Warning, TEXT("Boss has spawned at: %s"), *GetActorLocation().ToString());
}

void AGuardianLeviathanBoss::EnterNextStage()
{
    CurrentStage++;
}

void AGuardianLeviathanBoss::FireAttack()
{
    if (!FireMontage) return;

    UE_LOG(LogTemp, Log, TEXT("Boss performing Fire Breath"));
    enemyMesh->GetAnimInstance()->Montage_Play(FireMontage);
    enemyMesh->GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, FireMontage);

    // Start damage over time
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGuardianLeviathanBoss::ApplyFireDamage, 0.5f, true, 0.0f);
    GetWorldTimerManager().SetTimer(StopFireHandle, this, &AGuardianLeviathanBoss::StopFireBreath, 5.0f, false);
}

void AGuardianLeviathanBoss::ApplyFireDamage()
{
    TArray<AActor*> Players;
    GetOverlappingActors(Players, AAPlayerCharacter::StaticClass());

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
    GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AGuardianLeviathanBoss::ScreechAttack()
{
    if (!ScreechMontage) return;

    UE_LOG(LogTemp, Log, TEXT("Boss performing Screech"));
    enemyMesh->GetAnimInstance()->Montage_Play(ScreechMontage);
    enemyMesh->GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, ScreechMontage);

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

    UE_LOG(LogTemp, Log, TEXT("Boss performing Lunge"));
    enemyMesh->GetAnimInstance()->Montage_Play(LungeMontage);
    enemyMesh->GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, LungeMontage);

    // Movement logic can be expanded later
}

void AGuardianLeviathanBoss::TransitionOut()
{
    if (!TransitionMontage) return;

    UE_LOG(LogTemp, Log, TEXT("Boss transitioning out"));
    enemyMesh->GetAnimInstance()->Montage_Play(TransitionMontage);
    enemyMesh->GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, TransitionMontage);
}

void AGuardianLeviathanBoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("Montage ended: %s"), *Montage->GetName());
}

void AGuardianLeviathanBoss::MoveToNewHole()
{
    // List of 6 hardcoded spawn points (world locations)
    TArray<FVector> HoleLocations = {
        FVector(1700, 4930, 90),     // Placeholder for Hole 1
        FVector(1800, 4930, 90),   // Hole 2
        FVector(1900, 4930, 90),  // Hole 3
        FVector(1600, 4930, 90),   // Hole 4
        FVector(1500, 4930, 90),  // Hole 5
        FVector(1400, 4930, 90)  // Hole 6
    };

    // Avoid picking the same spot again
    int32 NewIndexSpot;
    do {
        NewIndexSpot = FMath::RandRange(0, HoleLocations.Num() - 1);
    } while (NewIndexSpot == LastHoleIndex && HoleLocations.Num() > 1);

    LastHoleIndex = NewIndexSpot;

    // Teleport
    SetActorLocation(HoleLocations[NewIndexSpot]);

    UE_LOG(LogTemp, Warning, TEXT("Boss moved to hole %d at %s"), NewIndexSpot, *HoleLocations[NewIndexSpot].ToString());
}

void AGuardianLeviathanBoss::OnBossDamaged(float CurrentHealth)
{
    /*AController* Controller = GetController();
    if (Controller)
    {
        AAIController* AIController = Cast<AAIController>(Controller);
        if (AIController && AIController->GetBlackboardComponent())
        {
            AIController->GetBlackboardComponent()->SetValueAsInt("CurrentStage", CurrentStage);
        }
    }*/


    if (CurrentStage == 1 && CurrentHealth <= 66.f)
    {
        CurrentStage = 2;
        UE_LOG(LogTemp, Warning, TEXT("Stage 2 started!"));
    }
    else if (CurrentStage == 2 && CurrentHealth <= 33.f)
    {
        CurrentStage = 3;
        UE_LOG(LogTemp, Warning, TEXT("Stage 3 started!"));
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