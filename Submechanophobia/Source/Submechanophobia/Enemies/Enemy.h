// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"

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
private:
	
public:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) ;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack() ;
	
	UPROPERTY(visibleAnywhere,BlueprintReadWrite, Category = "Components" )
	USkeletalMeshComponent* enemyMesh;
	UFloatingPawnMovement* MovementComponent;
};




