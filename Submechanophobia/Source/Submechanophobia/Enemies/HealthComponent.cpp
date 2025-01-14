// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::Initialize(uint8 MaxHealth_)
{
	maxHealth = MaxHealth_;
	currentHealth = maxHealth;
}

uint8 UHealthComponent::GetCurrentHealth()
{
	return currentHealth;
}

uint8 UHealthComponent::GetMaxHealth()
{
	return maxHealth;
}

void UHealthComponent::TakeDamage(uint8 Damage)
{
	// Clamp makes it so you cant go over or under the min or max
	currentHealth = FMath::Clamp(currentHealth- Damage, 0, maxHealth);
}

void UHealthComponent::AddHealth(uint8 addedHealth_)
{
	currentHealth = FMath::Clamp(currentHealth + addedHealth_, 0, maxHealth);
}

// allows you to set health to any value you want this can be over max  
void UHealthComponent::SetHealth(uint8 health)
{
	currentHealth = health;
}

inline void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}






