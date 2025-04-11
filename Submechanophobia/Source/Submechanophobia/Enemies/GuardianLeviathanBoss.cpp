#include "GuardianLeviathanBoss.h"
#include "Submechanophobia/Player/APlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

//static variables
float AGuardianLeviathanBoss::SharedHealth = 100.f;
float AGuardianLeviathanBoss::MaxSharedHealth = 100.f;
AGuardianLeviathanBoss* AGuardianLeviathanBoss::PrimaryBoss = nullptr;
TArray<int32> AGuardianLeviathanBoss::OccupiedHoleIndices;

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
    FireDamageHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FireDamageHitbox"));
    FireDamageHitbox->SetupAttachment(RootComponent);
    FireDamageHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FireDamageHitbox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    FireDamageHitbox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

    bReplicates = true;
    SetReplicatingMovement(true);
}

void AGuardianLeviathanBoss::BeginPlay()
{
    Super::BeginPlay();

    // Ensure clean state on play start
    if (!bIsPrimaryBoss)
    {
        PrimaryBoss = nullptr;
    }

    if (HasAuthority() && !PrimaryBoss)
    {
        PrimaryBoss = this;
        SharedHealth = MaxSharedHealth;
    }

    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsInt(FName("CurrentStage"), CurrentStage); // default = 1, after should follow what is set (when second serpent is created CurrentStage = 2)
        }
    }

    TArray<UNiagaraComponent*> NiagaraComponents;
    GetComponents<UNiagaraComponent>(NiagaraComponents);

    for (UNiagaraComponent* NiagaraComp : NiagaraComponents)
    {
        if (NiagaraComp->GetName().Contains("FlameVFX1"))
        {
            FlameEffect1 = NiagaraComp;
        }
        else if (NiagaraComp->GetName().Contains("FlameVFX2"))
        {
            FlameEffect2 = NiagaraComp;
        }
    }


}

void AGuardianLeviathanBoss::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //DOREPLIFETIME(AGuardianLeviathanBoss, FlameEffect1);
    //DOREPLIFETIME(AGuardianLeviathanBoss, FlameEffect2);
    DOREPLIFETIME(AGuardianLeviathanBoss, FireMontage);
    DOREPLIFETIME(AGuardianLeviathanBoss, TransitionMontage);
    DOREPLIFETIME(AGuardianLeviathanBoss, ReplicatedSharedHealth);
    DOREPLIFETIME(AGuardianLeviathanBoss, CurrentStage);
}

void AGuardianLeviathanBoss::OnRep_CurrentStage()
{
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

void AGuardianLeviathanBoss::EnterNextStage()
{
    CurrentStage++;
}

void AGuardianLeviathanBoss::OnRep_SharedHealth()
{
    // Keep the static in sync across all instances
    SharedHealth = ReplicatedSharedHealth;

    UE_LOG(LogTemp, Warning, TEXT("OnRep_SharedHealth triggered. Updated SharedHealth to %.2f"), SharedHealth);

    // Optional: trigger animations, VFX, sounds here if health drops at thresholds
    OnBossDamaged(SharedHealth);
}

void AGuardianLeviathanBoss::OnBossDamaged(float CurrentHealth)
{
    if (!HasAuthority() || !PrimaryBoss) return;

    UWorld* World = GetWorld();
    if (!World || HoleTransforms.Num() == 0) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    SpawnParams.bNoFail = true;
    SpawnParams.bDeferConstruction = false;

    // --- Stage 2 ---
    if (CurrentStage == 1 && CurrentHealth <= 66.f)
    {
        CurrentStage = 2;
        UE_LOG(LogTemp, Warning, TEXT("Stage 2 started!"));

        int32 SpawnIndex = FindAvailableHoleIndex();
        if (SpawnIndex == -1)
        {
            UE_LOG(LogTemp, Error, TEXT("No free holes available for spawning second serpent."));
            return;
        }

        const FSpawnHole& SpawnHole = HoleTransforms[SpawnIndex];

        AGuardianLeviathanBoss* NewSerpent = World->SpawnActor<AGuardianLeviathanBoss>(
            GetClass(),
            SpawnHole.Location,
            SpawnHole.Rotation,
            SpawnParams
        );

        if (NewSerpent)
        {
            NewSerpent->HoleTransforms = this->HoleTransforms;
            NewSerpent->LastHoleIndex = SpawnIndex;
            OccupiedHoleIndices.Add(SpawnIndex);

            NewSerpent->PrimaryBoss = this->PrimaryBoss;
            NewSerpent->CurrentStage = 2;
            NewSerpent->SetReplicates(true);
            NewSerpent->SetReplicatingMovement(true);

            UE_LOG(LogTemp, Warning, TEXT("Second serpent spawned at hole %d"), SpawnIndex);

            // Replicate spawn to clients
            Multicast_SpawnSerpent(SpawnHole.Location, SpawnHole.Rotation, 2, SpawnIndex);
        }
    }
    // --- Stage 3 ---
    else if (CurrentStage == 2 && CurrentHealth <= 33.f)
    {
        CurrentStage = 3;
        UE_LOG(LogTemp, Warning, TEXT("Stage 3 started!"));

        int32 SpawnIndex = FindAvailableHoleIndex();
        if (SpawnIndex == -1)
        {
            UE_LOG(LogTemp, Error, TEXT("No free holes available for spawning third serpent."));
            return;
        }

        const FSpawnHole& SpawnHole = HoleTransforms[SpawnIndex];

        AGuardianLeviathanBoss* NewSerpent = World->SpawnActor<AGuardianLeviathanBoss>(
            GetClass(),
            SpawnHole.Location,
            SpawnHole.Rotation,
            SpawnParams
        );

        if (NewSerpent)
        {
            NewSerpent->HoleTransforms = this->HoleTransforms;
            NewSerpent->LastHoleIndex = SpawnIndex;
            OccupiedHoleIndices.Add(SpawnIndex);

            NewSerpent->PrimaryBoss = this->PrimaryBoss;
            NewSerpent->CurrentStage = 3;
            NewSerpent->SetReplicates(true);
            NewSerpent->SetReplicatingMovement(true);

            UE_LOG(LogTemp, Warning, TEXT("Third serpent spawned at hole %d"), SpawnIndex);

            // Replicate spawn to clients
            Multicast_SpawnSerpent(SpawnHole.Location, SpawnHole.Rotation, 3, SpawnIndex);
        }
    }

    // --- Blackboard Update ---
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

    }

    return DamageAmount;
}

void AGuardianLeviathanBoss::ApplySharedDamage(float Amount)
{
    if (!IsValid(this) || this->IsActorBeingDestroyed())
    {
        UE_LOG(LogTemp, Error, TEXT("This serpent is invalid or pending kill!"));
        return;
    }

    if (!IsValid(PrimaryBoss))
    {
        UE_LOG(LogTemp, Error, TEXT("ApplySharedDamage: PrimaryBoss is invalid or destroyed!"));
        return;
    }

    SharedHealth -= Amount;
    SharedHealth = FMath::Clamp(SharedHealth, 0.f, MaxSharedHealth);
    ReplicatedSharedHealth = SharedHealth;

    UE_LOG(LogTemp, Warning, TEXT("[Shared Damage] Applied %.2f — Current: %.2f"), Amount, SharedHealth);

    UWorld* World = PrimaryBoss->GetWorld();

    if (SharedHealth <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Boss defeated — shared health depleted."));

        // Clean up all serpents
        TArray<AActor*> FoundSerpents;
        UGameplayStatics::GetAllActorsOfClass(World, AGuardianLeviathanBoss::StaticClass(), FoundSerpents);

        for (AActor* Serpent : FoundSerpents)
        {
            AGuardianLeviathanBoss* SerpentBoss = Cast<AGuardianLeviathanBoss>(Serpent);
            if (SerpentBoss)
            {
                // Stop Behavior Tree cleanly
                AAIController* AIController = Cast<AAIController>(SerpentBoss->GetController());
                if (AIController && AIController->BrainComponent)
                {
                    AIController->BrainComponent->StopLogic("Boss defeated");
                }

                // Free up the hole index
                if (SerpentBoss->LastHoleIndex >= 0)
                {
                    OccupiedHoleIndices.Remove(SerpentBoss->LastHoleIndex);
                }

                // Destroy the serpent
                SerpentBoss->Destroy();

                PrimaryBoss = nullptr;
            }
        }

        OccupiedHoleIndices.Empty(); // Clear tracking list

        //add  widgets to all players
        Multicast_OnBossDefeated();

    }
    if (PrimaryBoss)
    {
        PrimaryBoss->OnBossDamaged(SharedHealth);
    }

}

void AGuardianLeviathanBoss::FireAttack()
{

    if (HasAuthority())
    {
        Server_FireAttack();
    }
    else
    {
        Server_FireAttack(); // Clients will call the server
    }
}

void AGuardianLeviathanBoss::Server_FireAttack_Implementation()
{
    // Core logic stays server-side
    ApplySharedDamage(10.f);
    
    if (FireDamageHitbox)
    {
        FireDamageHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }

    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGuardianLeviathanBoss::ApplyFireDamage, 0.5f, true, 0.0f);
    GetWorldTimerManager().SetTimer(StopFireHandle, this, &AGuardianLeviathanBoss::StopFireBreath, 5.0f, false);

    // Broadcast effects to everyone
    Multicast_PlayFireAttack();
}

void AGuardianLeviathanBoss::Multicast_PlayFireAttack_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Multicast_PlayFireAttack called on %s. HasAuthority: %s, Role: %d"),
        *GetName(),
        HasAuthority() ? TEXT("true") : TEXT("false"),
        (int32)GetLocalRole());

    USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh || !FireMontage) return;

    UAnimInstance* Anim = Mesh->GetAnimInstance();
    if (!Anim) return;

    Anim->Montage_Play(FireMontage);

    if (FlameEffect1) FlameEffect1->Activate();
    if (FlameEffect2) FlameEffect2->Activate();

    UE_LOG(LogTemp, Warning, TEXT("Multicast_PlayFireAttack called on %s"), *GetName());
}

void AGuardianLeviathanBoss::ApplyFireDamage()
{
    if (!FireDamageHitbox)
    {
        UE_LOG(LogTemp, Error, TEXT("FireDamageHitbox is null!"));
        return;
    }

    TArray<AActor*> Players;
    FireDamageHitbox->GetOverlappingActors(Players, AAPlayerCharacter::StaticClass());

    for (AActor* Player : Players)
    {
        if (AAPlayerCharacter* Target = Cast<AAPlayerCharacter>(Player))
        {
            UGameplayStatics::ApplyDamage(Target, 10.0f, GetController(), this, nullptr);
        }
    }
}

void AGuardianLeviathanBoss::StopFireBreath()
{
    if (FlameEffect1) FlameEffect1->Deactivate();
    if (FlameEffect2) FlameEffect2->Deactivate();

    FireDamageHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AGuardianLeviathanBoss::ApplyScreechDamage()
{
    USphereComponent* ScreechHitbox = FindComponentByClass<USphereComponent>();
    if (!ScreechHitbox)
    {
        UE_LOG(LogTemp, Error, TEXT("ScreechHitbox (Sphere) not found!"));
        return;
    }

    TArray<AActor*> Players;
    ScreechHitbox->GetOverlappingActors(Players, AAPlayerCharacter::StaticClass());

    for (AActor* Player : Players)
    {
        if (AAPlayerCharacter* Target = Cast<AAPlayerCharacter>(Player))
        {
            UGameplayStatics::ApplyDamage(Target, 15.0f, GetController(), this, nullptr);
        }
    }
}

void AGuardianLeviathanBoss::ScreechAttack()
{
    if (!ScreechMontage) return;

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;

    UE_LOG(LogTemp, Log, TEXT("Boss performing Screech"));
    Anim->Montage_SetEndDelegate(MontageEndDelegate, ScreechMontage);
    Anim->Montage_Play(ScreechMontage);

    // Damage only — stun logic omitted per now
    TArray<AActor*> Players;
    GetOverlappingActors(Players, AAPlayerCharacter::StaticClass());

    // Play sound
    if (ScreechSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ScreechSound, GetActorLocation());
    }

    // Apply AOE Damage
    ApplyScreechDamage();
}

void AGuardianLeviathanBoss::ApplyLungeDamage()
{
    UCapsuleComponent* BossHitbox = FindComponentByClass<UCapsuleComponent>();
    if (!BossHitbox)
    {
        UE_LOG(LogTemp, Error, TEXT("BossHitbox not found!"));
        return;
    }

    TArray<AActor*> OverlappingPlayers;
    BossHitbox->GetOverlappingActors(OverlappingPlayers, AAPlayerCharacter::StaticClass());

    for (AActor* Player : OverlappingPlayers)
    {
        if (AAPlayerCharacter* Target = Cast<AAPlayerCharacter>(Player))
        {
            UGameplayStatics::ApplyDamage(Target, 25.0f, GetController(), this, nullptr);
            UE_LOG(LogTemp, Log, TEXT("Lunge hit a player!"));
        }
    }
}

void AGuardianLeviathanBoss::LungeAttack()
{
    if (!LungeMontage) return;

    UE_LOG(LogTemp, Log, TEXT("Boss performing Lunge"));

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (Anim)
    {
        Anim->Montage_Play(LungeMontage);
        Anim->Montage_SetEndDelegate(MontageEndDelegate, LungeMontage);
    }

    // Face the nearest player
    AActor* ClosestPlayer = nullptr;
    float ClosestDistance = FLT_MAX;

    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAPlayerCharacter::StaticClass(), FoundPlayers);

    for (AActor* Player : FoundPlayers)
    {
        float Dist = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
        if (Dist < ClosestDistance)
        {
            ClosestPlayer = Player;
            ClosestDistance = Dist;
        }
    }

    if (ClosestPlayer)
    {
        FVector Direction = (ClosestPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FRotator NewRot = Direction.Rotation();
        SetActorRotation(NewRot); // Face player

        // Move forward a bit in that direction (basic lunge)
        FVector LungeDestination = GetActorLocation() + Direction * 500.f; // Adjust strength as needed
        SetActorLocation(LungeDestination, true); // 'true' = sweep to avoid going through walls
    }

    // Do damage after lunging
    GetWorldTimerManager().SetTimerForNextTick(this, &AGuardianLeviathanBoss::ApplyLungeDamage);
}

void AGuardianLeviathanBoss::TransitionOut()
{
    if (HasAuthority())
    {
        Multicast_PlayTransitionOut();
    }

}

void AGuardianLeviathanBoss::Multicast_PlayTransitionOut_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("[Multicast_PlayTransitionOut] Called on %s - HasAuthority: %s, Role: %d"),
        *GetName(),
        HasAuthority() ? TEXT("true") : TEXT("false"),
        (int32)GetLocalRole());

    UE_LOG(LogTemp, Warning, TEXT("TransitionMontage is %s on %s"),
        TransitionMontage ? TEXT("VALID") : TEXT("NULL"),
        *GetName());

    if (!TransitionMontage || !enemyMesh) return;

    UE_LOG(LogTemp, Warning, TEXT("Multicast_PlayTransitionOut on %s"), *GetName());

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;

    Anim->Montage_Play(TransitionMontage);

}

void AGuardianLeviathanBoss::PlayTransitionIn()
{
    if (HasAuthority())
    {
        Multicast_PlayTransitionIn();
    }
}

void AGuardianLeviathanBoss::Multicast_PlayTransitionIn_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("[Multicast_PlayTransitionIn] Called on %s - HasAuthority: %s, Role: %d"),
        *GetName(),
        HasAuthority() ? TEXT("true") : TEXT("false"),
        (int32)GetLocalRole());

    UE_LOG(LogTemp, Warning, TEXT("TransitionMontage is %s on %s"),
        TransitionMontage ? TEXT("VALID") : TEXT("NULL"),
        *GetName());
    
    if (!TransitionMontage || !enemyMesh) return;

    UE_LOG(LogTemp, Warning, TEXT("Multicast_PlayTransitionIn on %s"), *GetName());

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;

    const float MontageLength = TransitionMontage->GetPlayLength();

    // Ensure the animation starts at the end and plays backward
    Anim->Montage_Play(TransitionMontage, -1.0f);
    Anim->Montage_SetPosition(TransitionMontage, MontageLength);

}

void AGuardianLeviathanBoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("Montage ended: %s"), *Montage->GetName());
}

void AGuardianLeviathanBoss::MoveToNewHole()
{
    //prevent client from running
    if (!HasAuthority()) return;

    if (HoleTransforms.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("HoleTransforms is empty! Cannot move."));
        return;
    }

    int32 NewIndexSpot = -1;
    int32 Attempts = 10; // safety cap

    while (Attempts-- > 0)
    {
        int32 TestIndex = FMath::RandRange(0, HoleTransforms.Num() - 1);

        if (!OccupiedHoleIndices.Contains(TestIndex))
        {
            NewIndexSpot = TestIndex;
            break;
        }
    }

    if (NewIndexSpot == -1)
    {
        UE_LOG(LogTemp, Error, TEXT("No available hole found after retries."));
        return;
    }

    if (LastHoleIndex >= 0)
    {
        OccupiedHoleIndices.Remove(LastHoleIndex);
    }

    // Update index
    LastHoleIndex = NewIndexSpot;
    OccupiedHoleIndices.Add(NewIndexSpot);

    const FSpawnHole& Target = HoleTransforms[NewIndexSpot];
    SetActorLocationAndRotation(Target.Location, Target.Rotation);
}



float AGuardianLeviathanBoss::GetSharedHealth()
{
    return SharedHealth;
}

int32 AGuardianLeviathanBoss::FindAvailableHoleIndex()
{
    const int32 NumHoles = HoleTransforms.Num();
    if (NumHoles == 0) return -1;

    TArray<int32> Indices;
    for (int32 i = 0; i < NumHoles; ++i)
    {
        if (!OccupiedHoleIndices.Contains(i))
        {
            Indices.Add(i);
        }
    }

    if (Indices.Num() == 0)
    {
        return -1; // No free holes
    }

    const int32 RandomIndex = FMath::RandRange(0, Indices.Num() - 1);
    return Indices[RandomIndex];
}

void AGuardianLeviathanBoss::Multicast_OnBossDefeated_Implementation()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());

        if (WinWidgetClass)
        {
            UUserWidget* WinWidget = CreateWidget<UUserWidget>(PC, WinWidgetClass);
            if (WinWidget)
            {
                WinWidget->AddToViewport();
            }
        }
    }
}


void AGuardianLeviathanBoss::Multicast_SpawnSerpent_Implementation(FVector Location, FRotator Rotation, int32 Stage, int32 HoleIndex)
{
    if (!HasAuthority())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AGuardianLeviathanBoss* NewSerpent = GetWorld()->SpawnActor<AGuardianLeviathanBoss>(
            GetClass(),
            Location,
            Rotation,
            SpawnParams
        );

        if (NewSerpent)
        {
            NewSerpent->HoleTransforms = this->HoleTransforms;
            NewSerpent->LastHoleIndex = HoleIndex;
            OccupiedHoleIndices.Add(HoleIndex);

            NewSerpent->PrimaryBoss = this->PrimaryBoss; // Still static but consistent on clients
            NewSerpent->CurrentStage = Stage;
            NewSerpent->SetReplicates(true);
            NewSerpent->SetReplicatingMovement(true);

            UE_LOG(LogTemp, Warning, TEXT("Client: Serpent for Stage %d spawned at hole %d"), Stage, HoleIndex);
        }
    }
}
