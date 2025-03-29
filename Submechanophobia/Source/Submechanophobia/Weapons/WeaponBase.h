// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h" 
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "WeaponBase.generated.h"

class ASubmechanophobiaCharacter;

UCLASS()
class SUBMECHANOPHOBIA_API AUWeaponBase : public AActor
{
public:
	GENERATED_BODY()
	


	AUWeaponBase();

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
	
	UPROPERTY(EditAnywhere,blueprintReadOnly,Category = "Weapons")
	bool isPrimary;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "Weapons")
	TObjectPtr<USkeletalMeshComponent> weaponMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "Weapons")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY()
	bool IsPickedUp;

	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
};

