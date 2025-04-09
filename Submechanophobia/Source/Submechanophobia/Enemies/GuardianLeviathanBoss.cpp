// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianLeviathanBoss.h"
#include "Submechanophobia/Player/APlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
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
}

void AGuardianLeviathanBoss::BeginPlay()
{
    Super::BeginPlay();
    
    // Ensure clean state on play start
    if (!bIsPrimaryBoss)
    {
        PrimaryBoss = nullptr;
    }

    if (!PrimaryBoss)
    {
        PrimaryBoss = this; // First one becomes the one that handles stage logic
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

    if (!FlameEffect1 || !FlameEffect2)
    {
        UE_LOG(LogTemp, Error, TEXT("FlameEffect Niagara Components are not initialized!"));
    }

    ReplicatedSharedHealth = SharedHealth;
 
}

void AGuardianLeviathanBoss::EnterNextStage()
{
    CurrentStage++;
}

void AGuardianLeviathanBoss::FireAttack()
{
    
    USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh) return;

    UAnimInstance* Anim = Mesh->GetAnimInstance();
    if (!Anim || !FireMontage) return;

    Anim->Montage_Play(FireMontage);

    // SERVER-ONLY visual activation
    if (FlameEffect1) FlameEffect1->Activate();
    if (FlameEffect2) FlameEffect2->Activate();
    // this will trigger onRep
    bShouldPlayFireVFX = true;
    

    //USED FOR TESTING
    ApplySharedDamage(10.f);

    UE_LOG(LogTemp, Warning, TEXT("Boss current shared health: %.2f"), GetSharedHealth());
    // === END TEST ===

    if (FireDamageHitbox)
    {
        FireDamageHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }

    // Start damage over time
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGuardianLeviathanBoss::ApplyFireDamage, 0.5f, true, 0.0f);
    GetWorldTimerManager().SetTimer(StopFireHandle, this, &AGuardianLeviathanBoss::StopFireBreath, 5.0f, false);
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

    // SERVER deactivates directly
    if (FlameEffect1) FlameEffect1->Deactivate();
    if (FlameEffect2) FlameEffect2->Deactivate();
    bShouldStopFireVFX = true;
    

    FireDamageHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GetWorldTimerManager().ClearTimer(FireTimerHandle);
    
    bShouldPlayFireVFX = false;
    bShouldStopFireVFX = false;
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
    /*if (!TransitionMontage) return;

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;*/
    USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh) return;

    UAnimInstance* Anim = Mesh->GetAnimInstance();
    if (!Anim || !TransitionMontage) return;

    Anim->Montage_Play(TransitionMontage);

}

void AGuardianLeviathanBoss::PlayTransitionIn()
{
    if (!TransitionMontage || !enemyMesh) return;

    UE_LOG(LogTemp, Warning, TEXT("Boss playing Transition IN (reverse)"));

    UAnimInstance* Anim = enemyMesh->GetAnimInstance();
    if (!Anim) return;

    Anim->Montage_Play(TransitionMontage, 1.0f);
    Anim->Montage_SetPlayRate(TransitionMontage, -1.0f);
    Anim->Montage_JumpToSection(FName("Default"), TransitionMontage); // or any known section/end
}

void AGuardianLeviathanBoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("Montage ended: %s"), *Montage->GetName());
}

void AGuardianLeviathanBoss::MoveToNewHole()
{
    if (HoleTransforms.Num() == 0) return;

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

    if (NewIndexSpot == -1) return; // Failed to find a valid hole

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

void AGuardianLeviathanBoss::OnBossDamaged(float CurrentHealth)
{
   
    if (!PrimaryBoss) return;

    UWorld* World = GetWorld();
    if (!World || HoleTransforms.Num() == 0) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

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

            UE_LOG(LogTemp, Warning, TEXT("Second serpent spawned at hole %d"), SpawnIndex);
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

            UE_LOG(LogTemp, Warning, TEXT("Third serpent spawned at hole %d"), SpawnIndex);
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
        ReplicatedSharedHealth = SharedHealth;

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

       // 👇 Add Win Widget and disable player control
       APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
       if (PC)
       {
           PC->SetIgnoreMoveInput(true);
           PC->SetIgnoreLookInput(true);
           PC->bShowMouseCursor = true;
           PC->SetInputMode(FInputModeUIOnly());

           if (WinWidgetClass) // ← UPROPERTY for the Widget
           {
               UUserWidget* WinWidget = CreateWidget<UUserWidget>(PC, WinWidgetClass);
               if (WinWidget)
               {
                   WinWidget->AddToViewport();
               }
           }
       }

    }
    if (PrimaryBoss)
    {
        PrimaryBoss->OnBossDamaged(SharedHealth);
    }
   
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

void AGuardianLeviathanBoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGuardianLeviathanBoss, CurrentStage);
    DOREPLIFETIME(AGuardianLeviathanBoss, bIsPrimaryBoss);
    DOREPLIFETIME(AGuardianLeviathanBoss, ReplicatedSharedHealth);
    DOREPLIFETIME(AGuardianLeviathanBoss, bShouldPlayFireVFX);
    DOREPLIFETIME(AGuardianLeviathanBoss, bShouldStopFireVFX);
}

void AGuardianLeviathanBoss::OnRep_SharedHealth()
{
    SharedHealth = ReplicatedSharedHealth;
    UE_LOG(LogTemp, Log, TEXT("[Client] SharedHealth replicated: %.2f"), SharedHealth);
}


void AGuardianLeviathanBoss::OnRep_PlayFireVFX()
{
    if (HasAuthority()) return; // Avoid activating twice on server

    if (FlameEffect1 && FlameEffect1->IsRegistered())
        FlameEffect1->Activate();

    if (FlameEffect2 && FlameEffect2->IsRegistered())
        FlameEffect2->Activate();
}

void AGuardianLeviathanBoss::OnRep_StopFireVFX()
{
    if (HasAuthority()) return; // Server already did this

    if (FlameEffect1 && FlameEffect1->IsRegistered())
        FlameEffect1->Deactivate();

    if (FlameEffect2 && FlameEffect2->IsRegistered())
        FlameEffect2->Deactivate();
}