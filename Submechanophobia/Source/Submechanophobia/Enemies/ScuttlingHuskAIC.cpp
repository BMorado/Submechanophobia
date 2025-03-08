// Fill out your copyright notice in the Description page of Project Settings.


#include "ScuttlingHuskAIC.h"

AScuttlingHuskAIC::AScuttlingHuskAIC()
{
	//static ConstructorHelpers::FObjectFinder<UBehaviorTreeComponent> BTree(TEXT("/Game/AI/ScuttlingHuskAI/BT_ScuttlingHusk.BT_ScuttlingHusk"));
}

void AScuttlingHuskAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}
