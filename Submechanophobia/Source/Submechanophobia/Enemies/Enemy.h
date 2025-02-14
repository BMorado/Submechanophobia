// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
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
	uint8 damage;
public:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) ;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	virtual void Attack() ;
	
	UPROPERTY(visibleAnywhere,BlueprintReadWrite, Category = "Components" )
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(visibleAnywhere,BlueprintReadWrite, Category = "Components" )
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(visibleAnywhere,BlueprintReadWrite, Category = "Components" )
	TObjectPtr<USkeletalMeshComponent>  enemyMesh;

	// allows AIController to move the pawn
	UPROPERTY()
	TObjectPtr<UFloatingPawnMovement>  MovementComponent;
	
	UPROPERTY()
	TObjectPtr<UHealthComponent>  HealthComponent;

	uint8 GetDamage() const{return damage;}
	void SetDamage(const uint8 damage_){damage = damage_;}

	FTimerHandle timer;
};




