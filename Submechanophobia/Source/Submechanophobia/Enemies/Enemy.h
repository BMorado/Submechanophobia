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
// Event Dispatchers (Multi Cast Delegates)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageEnd);
UCLASS()
class SUBMECHANOPHOBIA_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	uint8 damage;
	bool isDamagable = true;
public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable, Category = "Functions")
	virtual void PlayAttackAnim() ;

	UFUNCTION( BlueprintCallable,Category = "Functions")
	virtual void OnTakeDamage();
	
	UFUNCTION(BlueprintCallable, Category = "Functions")
	virtual void Attack() ;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttackEnd OnAttackEnd;
	
	
	
	
	uint8 GetDamage() const{return damage;}
	void SetDamage(const uint8 damage_){damage = damage_;}

	
	UPROPERTY(visibleAnywhere,BlueprintReadWrite, Category = "Components" )
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(visibleAnywhere,BlueprintReadWrite, Category = "Components" )
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(visibleAnywhere,BlueprintReadWrite, Category = "Components" )
	TObjectPtr<USkeletalMeshComponent>  enemyMesh;

	UPROPERTY()
	TObjectPtr<UHealthComponent>  HealthComponent;

	// allows AIController to move the pawn
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UFloatingPawnMovement>  MovementComponent;
	
	UPROPERTY()
	FOnDamageEnd OnDamageEnd;

	FTimerHandle timer;

	FVector SpawnLocation;
	
};




