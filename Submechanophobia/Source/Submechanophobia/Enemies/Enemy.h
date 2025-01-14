// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "string"
#include "Enemy.generated.h"

UCLASS()
class SUBMECHANOPHOBIA_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned

	

public:
	virtual void BeginPlay();
	// Called every frame
	virtual void Tick(float DeltaTime) ;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
