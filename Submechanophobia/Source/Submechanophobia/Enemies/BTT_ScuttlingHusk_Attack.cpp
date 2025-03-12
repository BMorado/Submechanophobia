// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ScuttlingHusk_Attack.h"

#include "AIController.h"

EBTNodeResult::Type UBTT_ScuttlingHusk_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = &OwnerComp;
	AEnemy* enemy = dynamic_cast<AEnemy*> (OwnerComp.GetAIOwner()->GetOwner());
	if (enemy != nullptr)
	{
		
			UE_LOG(LogTemp, Log, TEXT("Task Owner: %s"), *enemy->GetName());
		enemy->OnAttackEnd.AddDynamic(this,&UBTT_ScuttlingHusk_Attack::OnAttackFinished);
		enemy->Attack();
	}
	return EBTNodeResult::Succeeded;
}

void UBTT_ScuttlingHusk_Attack::OnAttackFinished() 
{
	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}
