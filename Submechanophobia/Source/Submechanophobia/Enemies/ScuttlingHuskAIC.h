// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ScuttlingHuskAIC.generated.h"

// Forward Declaration
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API AScuttlingHuskAIC : public AAIController
{
	GENERATED_BODY()

	AScuttlingHuskAIC();
	
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent ;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

private:
	UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
};
