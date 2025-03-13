// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ScuttlingHusk_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API UBTT_ScuttlingHusk_Attack : public UBTTask_BlackboardBase
{
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UFUNCTION()
	void OnAttackFinished();
	GENERATED_BODY()
};
