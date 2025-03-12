// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "WeaponBase.generated.h"

class ASubmechanophobiaCharacter;

UCLASS()
class SUBMECHANOPHOBIA_API UWeaponBase : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	unsigned int ammo;
	
	UPROPERTY()
	class UInputMappingContext* FireMappingContext;
	
	UPROPERTY()
	class UInputAction* FireAction;

	// function to be overriden for each weapon  
	UFUNCTION()
	virtual void Fire();

	//Attaches component to Character
	UFUNCTION(BlueprintCallable)
	virtual bool AttachToCharacter(ASubmechanophobiaCharacter* TargetCharacter);

	virtual void OnGenerateOverlapEventsChanged() override;
	
	//TSubclassOf<ACharacter> BlueprintCharacterClass
	
	/** The Character holding this weapon*/
	ASubmechanophobiaCharacter* Character;
};

