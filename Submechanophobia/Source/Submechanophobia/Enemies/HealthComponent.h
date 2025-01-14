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
	uint8 currentHealth;
	uint8 maxHealth;
public:	
	// Sets default values for this component's properties
	UHealthComponent();
	void Initialize(uint8 MaxHealth_);
	uint8 GetCurrentHealth();
	uint8 GetMaxHealth();
	void TakeDamage(uint8 Damage);
	void AddHealth(uint8 health);
	void SetHealth(uint8 health);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	
	
		
};

