// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GuardianLeviathanBoss.generated.h"

/**
 * 
 */
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    int CurrentStage = 1;

    UFUNCTION(BlueprintCallable)
    void EnterNextStage();

    UFUNCTION()
    void OnBossDamaged(float CurrentHealth);


    // --- AI Controller Reference if using C++ instead of BP ---
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    class AGuardianLeviathanAIC* BossAIC;*/

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

    UFUNCTION(BlueprintCallable)
    void ScreechAttack();

    UFUNCTION(BlueprintCallable)
    void LungeAttack();

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator, AActor* DamageCauser) override;

    // --- Movements -- 
    UFUNCTION(BlueprintCallable)
    void TransitionOut();

    UFUNCTION(BlueprintCallable)
    void MoveToNewHole();

    int32 LastHoleIndex = -1;  // Stores previously used index

    // --- Internal Helpers ---
    void ApplyFireDamage();
    void StopFireBreath();

    // --- Mesh stuff ---

   /* UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* enemyMesh;*/

};