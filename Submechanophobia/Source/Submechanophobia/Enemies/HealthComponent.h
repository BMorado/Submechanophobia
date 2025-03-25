// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBMECHANOPHOBIA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	// uint8 is unsigned and between 0 - 255
	float currentHealth;
	float maxHealth;
public:	
	// Sets default values for this component's properties
	UHealthComponent();
	void SetMaxHealth(float MaxHealth_);
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	void TakeDamage(float damage_);
	void AddHealth(float health_);
	void SetHealth(float health_);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	
	
		
};

