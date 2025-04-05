// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "HealthComponent.h"
#include "Animation/AnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GuardianLeviathanBoss.generated.h"


USTRUCT(BlueprintType)
struct SUBMECHANOPHOBIA_API FSpawnHole
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Movement")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Movement")
    FRotator Rotation;
};

UCLASS()
class SUBMECHANOPHOBIA_API AGuardianLeviathanBoss : public AEnemy
{
    GENERATED_BODY()

public:
    AGuardianLeviathanBoss();

protected:
    virtual void BeginPlay() override;

public:
    // --- Stage Tracking ---
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    //float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    int CurrentStage = 1;

    UFUNCTION(BlueprintCallable)
    void EnterNextStage();

    static float SharedHealth;
    static float MaxSharedHealth;
    static AGuardianLeviathanBoss* PrimaryBoss; // Used to run stage transitions only

    // --- Anim Montages ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Animation")
    UAnimMontage* FireMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Animation")
    UAnimMontage* ScreechMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Animation")
    UAnimMontage* LungeMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Animation")
    UAnimMontage* TransitionMontage;

    // --- Animation Events ---
    FOnMontageEnded MontageEndDelegate;

    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    // --- Timer Handles ---
    FTimerHandle FireTimerHandle;
    FTimerHandle StopFireHandle;

    // --- Attack Calls (for BT or Blueprint) ---
    UFUNCTION(BlueprintCallable)
    void FireAttack();

    void ApplyScreechDamage();
    UFUNCTION(BlueprintCallable)
    void ScreechAttack();

    void ApplyLungeDamage(); // Called on impact or during animation
    UFUNCTION(BlueprintCallable)
    void LungeAttack();

    UFUNCTION(BlueprintCallable)
    void TransitionOut();

    // --- Internal Helpers ---
    void ApplyFireDamage();
    void StopFireBreath();

    UFUNCTION(BlueprintCallable)
    void MoveToNewHole();

    void OnBossDamaged(float CurrentHealth);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable)
    void ApplySharedDamage(float Amount);

    UFUNCTION(BlueprintCallable)
    static float GetSharedHealth();

    // --- Screech Sound ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Audio")
    USoundBase* ScreechSound;

    // Static tracking of occupied holes (shared across all serpents)
    static TArray<int32> OccupiedHoleIndices;

    // --- New Hole Transform Setup ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Movement")
    TArray<FSpawnHole> HoleTransforms;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Movement")
    int32 LastHoleIndex = -1;

    int32 FindAvailableHoleIndex();
};
