// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "GuardianLeviathanBoss.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SUBMECHANOPHOBIA_API AGuardianLeviathanBoss : public AEnemy
{
	GENERATED_BODY()
	
public:
    AGuardianLeviathan();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    class AGuardianLeviathanAIC* BossAIC;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    int CurrentStage;

    void TakeDamage(float DamageAmount);
    void EnterNextStage();

    UFUNCTION(BlueprintCallable, Category = "Boss Actions")
    void StartFireBreath();
    void ApplyFireDamage();
    void StopFireBreath();

    UFUNCTION(BlueprintCallable, Category = "Boss Actions")
    void StartScreech();

    UFUNCTION(BlueprintCallable, Category = "Boss Actions")
    void LungeAtTarget(AActor* Target);

};
