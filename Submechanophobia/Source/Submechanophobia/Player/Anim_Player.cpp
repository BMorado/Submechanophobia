// Fill out your copyright notice in the Description page of Project Settings.


#include "Submechanophobia/player/Anim_Player.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Submechanophobia/Player/APlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnim_Player::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerCharacter = Cast<AAPlayerCharacter>(TryGetPawnOwner());
}

void UAnim_Player::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (PlayerCharacter)
	{
		FVector Velocity = PlayerCharacter->GetVelocity();
		Velocity.Z =0;

		Speed = Velocity.Size();

		bAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;

		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
		if (PlayerCharacter->GetCurrentWeapon())
		{
			bHasPistol = !PlayerCharacter->GetCurrentWeapon()->isPrimary;
		}
		bHasFired = PlayerCharacter->hasFired;
	}
}