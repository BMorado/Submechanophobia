// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Submechanophobia/SubmechanophobiaCharacter.h"

void UPistol::Fire()
{
	if (ammo > 0)
	{
		FVector start = Character->GetActorLocation();
		FVector camForwardVector = Character->GetFirstPersonCameraComponent()->GetForwardVector();

		start = FVector(start.X + (camForwardVector.X * 100 ), start.Y + (camForwardVector.Y * 100 ), start.Z + (camForwardVector.Z * 100 ));
		FVector end = start + (camForwardVector * 1000);
		DrawDebugLine(GetWorld(),start,end,FColor::Red,false,2.0f,0.0f,10.0f);

		
		ammo--; 
	}
	
	
	
}

bool UPistol::AttachToCharacter(ASubmechanophobiaCharacter* TargetCharacter)
{
	return Super::AttachToCharacter(TargetCharacter);
}
