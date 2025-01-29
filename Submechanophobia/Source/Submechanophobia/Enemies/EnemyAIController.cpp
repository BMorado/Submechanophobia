// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	// Setup sight component
	/*SightConfig->SightRadius = 3500.0f; // How far it can see
	SightConfig->LoseSightRadius = 3500.0f; // if player is already seen how far they have to not be seen 
	SightConfig->PeripheralVisionAngleDegrees = 60.0f; // The angel it can see*/
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SetPerceptionComponent(*PerceptionComponent);
	
}

void AEnemyAIController::OnPerceptionUpdated(TArray<AActor*> UpdatedActors)
{
	for (AActor* UpdatedActor : UpdatedActors)
	{
		if (UpdatedActor == GetWorld()->GetFirstPlayerController()->GetOwner())
		{
			MoveToActor(UpdatedActor,1.0f);
			UE_LOG(LogTemp,Warning,TEXT("Shit did work"));
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Shit did not work"));
		}
	}
}
