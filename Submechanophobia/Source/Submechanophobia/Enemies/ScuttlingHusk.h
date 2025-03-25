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
	UFUNCTION()
	void DamagedEnd();
	
	virtual void Attack() override;
	virtual void PlayAttackAnim() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	FOnMontageEnded AttackMontageEndDelegate;
	FOnMontageEnded DamagedMontageEndDelegate;
	
	UPROPERTY(VisibleAnywhere,Category = "Animation")
	TObjectPtr<UAnimMontage> attackMontage;

	UPROPERTY(VisibleAnywhere,Category = "Animation")
	TObjectPtr<UAnimMontage> damagedMontage;
	
	uint8 damage = 10;
	FTimerHandle Timer;
	


	
};
