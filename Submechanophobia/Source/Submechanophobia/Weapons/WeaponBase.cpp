// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Submechanophobia/SubmechanophobiaCharacter.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



void UWeaponBase::Fire()
{
	if (ammo > 0)
	{
		
		--ammo; 
	}
	
}

bool UWeaponBase::AttachToCharacter(ASubmechanophobiaCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	//// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UWeaponBase>())
	{
		return false;
	}

	//// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	//// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	//// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}
		

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponBase::Fire);
		}
	}

	return true;
}

void UWeaponBase::OnGenerateOverlapEventsChanged()
{
	Super::OnGenerateOverlapEventsChanged();
	// Get overlap actor
	// cast to rut player character
	// player -> change gun(damage, mesh, fire rate)
};
