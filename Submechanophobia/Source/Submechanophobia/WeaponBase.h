// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API UWeaponBase : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	unsigned int Ammo; 
	
	UPROPERTY()
	class UInputMappingContext* FireMappingContext;

	UPROPERTY()
	class UInputAction* FireAction;
	
	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void Reload();

	
};
