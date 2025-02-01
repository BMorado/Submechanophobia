// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "DrownedCrew.generated.h"

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API ADrownedCrew : public AEnemy
{
	GENERATED_BODY()
	ADrownedCrew();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void Attack() override;
	
	uint8 damage = 10;
	FTimerHandle Timer;
};
