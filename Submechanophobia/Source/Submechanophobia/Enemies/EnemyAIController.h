// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"


/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
private:
	AEnemyAIController();
	// AI Perception Component
	/*UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AI",meta=(AllowPrivateAccess=true))
	UAIPerceptionComponent* PerceptionComponent;*/

	
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;
	UFUNCTION()
	void OnPerceptionUpdated(TArray<AActor*> UpdatedActors);

	
};
