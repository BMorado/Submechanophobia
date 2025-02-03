// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "LampreyMaw.generated.h"

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API ALampreyMaw : public AEnemy
{
	GENERATED_BODY()

	ALampreyMaw();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	uint8 damage = 10;
	
};
