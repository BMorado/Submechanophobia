// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardianLeviathanAIC.generated.h"

/**
 * 
 */
UCLASS()
class SUBMECHANOPHOBIA_API AGuardianLeviathanAIC : public AAIController
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BossBehaviorTree; //RENAME?

    void StartBehaviorTree();  //THIS NEEDS TO BE CODED AND CALLED SOMEWHERE
	
};
