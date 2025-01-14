// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "HealthComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ScuttlingHusk.generated.h"

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API AScuttlingHusk : public AEnemy
{
	GENERATED_BODY()
	AScuttlingHusk();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	USceneComponent* Root;
	UPROPERTY(visibleAnywhere, category = Mesh )
	USkeletalMeshComponent* enemyMesh;
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	UAnimBlueprint* animBlueprint;
};
