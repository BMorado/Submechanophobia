// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "Submechanophobia/Player/APlayerCharacter.h"


AUWeaponBase::AUWeaponBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetVisibility(false);
	SphereComponent->ShapeColor = FColor::Green;

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Enemy Mesh"));
}



void AUWeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsPickedUp == false)
	{
		AAPlayerCharacter* temp = dynamic_cast<AAPlayerCharacter*>(OtherActor);
		if (temp != nullptr)
		{
			temp->AddWeapon(this);
			IsPickedUp = true;
		}
			
	}
}
