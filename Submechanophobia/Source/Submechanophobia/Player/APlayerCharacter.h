// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Submechanophobia/Weapons/WeaponBase.h"
#include "APlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(Blueprintable)
class SUBMECHANOPHOBIA_API AAPlayerCharacter : public ACharacter 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
	void AddWeapon( AUWeaponBase* weapon);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly); 
	//UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;


	//controls 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* SwapToPrimaryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* SwapToSecondaryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* ReloadAction;
	
	//Animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation");
	UAnimMontage* MeleeAttackMontage;

	FTimerHandle UnusedHandle;

	// Weapons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "gameplay");
	AUWeaponBase* currentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "gameplay");
	AUWeaponBase* PrimaryWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "gameplay");
	AUWeaponBase* SecondaryWeapon; 

	FHitResult* HitResult;

	bool canShoot = true; 
	
	
	void StartJump();
	void StopJump();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void FireWeapon();
	void SwapWeaponPrimary();
	void SwapWeaponSecondary();
	void Reload(); 
	void WeaponFireDelay(); 

	void RayCast();
	
	
	
};
