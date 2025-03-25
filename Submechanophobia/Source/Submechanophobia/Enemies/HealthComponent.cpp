// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::SetMaxHealth(float MaxHealth_)
{
	maxHealth = MaxHealth_;
	currentHealth = maxHealth;
}

float UHealthComponent::GetCurrentHealth() const
{
	return currentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return maxHealth;
}

void UHealthComponent::TakeDamage(float damage_)
{
	// Clamp makes it so you cant go over or under the min or max
	currentHealth = FMath::Clamp(currentHealth- damage_, 0, maxHealth);
}

void UHealthComponent::AddHealth(float health_)
{
	currentHealth = FMath::Clamp(currentHealth + health_, 0, maxHealth);
}

// allows you to set health to any value you want this can be over max  
void UHealthComponent::SetHealth(float health_)
{
	currentHealth = health_;
}

inline void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}






