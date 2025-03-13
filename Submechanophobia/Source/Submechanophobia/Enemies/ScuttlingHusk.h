// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "HealthComponent.h"
#include "EnemyAIController.h"
#include "ScuttlingHusk.generated.h"

/**
 * 
 */



UCLASS()
class SUBMECHANOPHOBIA_API AScuttlingHusk : public AEnemy
{
private:
	GENERATED_BODY()
	AScuttlingHusk();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void AttackEnd();
	
	virtual void Attack() override;
	virtual void PlayAttackAnim() override;
	

	FOnMontageEnded MontageEndDelegate;
	UPROPERTY(VisibleAnywhere,Category = "Animation")
	TObjectPtr<UAnimMontage> attackMontage;
	
	uint8 damage = 10;
	FTimerHandle Timer;
	


	
};
