// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "Submechanophobia/Player/APlayerCharacter.h"


void AUWeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Cast<AAPlayerCharacter>(OtherActor)->AddWeapon(this); 
}
