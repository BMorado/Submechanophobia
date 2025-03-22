// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GuardianLeviathanBoss.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SUBMECHANOPHOBIA_API AGuardianLeviathanBoss : public AEnemy
{
	GENERATED_BODY()
	
public:
    AGuardianLeviathanBoss();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    class AGuardianLeviathanAIC* BossAIC;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    int CurrentStage;
    


   

    UFUNCTION(BlueprintCallable, Category = "Boss Actions")

    void EnterNextStage();
    
    FTimerHandle FireTimerHandle;

    FTimerHandle StopFireHandle;

    UFUNCTION(BlueprintCallable, Category = "Boss Actions")
    void StartFireBreath();
    UFUNCTION(BlueprintCallable, Category = "Boss Actions")

    void ApplyFireDamage();
    UFUNCTION(BlueprintCallable, Category = "Boss Actions")

    void StopFireBreath();

    UFUNCTION(BlueprintCallable, Category = "Boss Actions")
    void StartScreech();

    UFUNCTION(BlueprintCallable, Category = "Boss Actions")
    void LungeAtTarget(AActor* Target);

};
