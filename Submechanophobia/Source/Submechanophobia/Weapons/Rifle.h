// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API ARifle : public AUWeaponBase
{
	GENERATED_BODY()
	ARifle();

public:
	
	
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
