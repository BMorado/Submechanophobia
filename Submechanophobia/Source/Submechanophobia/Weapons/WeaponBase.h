// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "WeaponBase.generated.h"

class ASubmechanophobiaCharacter;

UCLASS()
class SUBMECHANOPHOBIA_API AUWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	
	// ammo remaining outside loaded magazine 
	UPROPERTY(EditAnywhere,blueprintReadOnly,Category = "Weapons")
	 uint8 reserveAmmo;
	
	UPROPERTY(EditAnywhere,blueprintReadOnly,Category = "Weapons")
	 uint8 magazineAmmo;

	UPROPERTY(EditAnywhere,blueprintReadOnly,Category = "Weapons")
	float fireRate;

	UPROPERTY(EditAnywhere,blueprintReadOnly,Category = "Weapons")
	float accuracy; 
	
	UPROPERTY(EditAnywhere,blueprintReadOnly,Category = "Weapons")
	float damage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "Weapons")
	TObjectPtr<USkeletalMeshComponent> weaponMesh;
	
};

