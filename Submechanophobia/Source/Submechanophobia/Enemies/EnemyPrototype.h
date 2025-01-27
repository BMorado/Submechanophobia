// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyPrototype.generated.h"

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API AEnemyPrototype : public AEnemy
{
	GENERATED_BODY()
	AEnemyPrototype();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
};
