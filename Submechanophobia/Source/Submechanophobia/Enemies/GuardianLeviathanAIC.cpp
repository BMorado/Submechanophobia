// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/GuardianLeviathanAIC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

void AGuardianLeviathanAIC::BeginPlay()
{
    Super::BeginPlay();

    if (BossBehaviorTree)
    {
        RunBehaviorTree(BossBehaviorTree);
    }
}
