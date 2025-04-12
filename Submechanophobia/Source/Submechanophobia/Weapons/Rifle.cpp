// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "Submechanophobia/Player/APlayerCharacter.h"

ARifle::ARifle()
{
	reserveAmmo = 50;
	
	magazineAmmo = 25;
	
	fireRate = 1.0f;
	
	bulletSpread = FVector(FMath::RandRange(-50.0f, 50.0f),FMath::RandRange(-50.0f, 50.0f),FMath::RandRange(-50.0f, 50.0f));
	
	damage = 25.0f;
	
	magazineCapacity = 25; 
	
	isPrimary = true;

	IsPickedUp = false;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARifle::OnOverlapBegin);

	SphereComponent->SetRelativeScale3D(FVector(1.0f,1.0f, 1.0f));
	SphereComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	SphereComponent->SetRelativeLocation(FVector(0, 0, 0));

	//SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetVisibility(true);
	
	SphereComponent->ShapeColor = FColor::Green;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset(TEXT("/Game/Player/Weapons/railgun.railgun"));

	if (WeaponMeshAsset.Succeeded())
	{
		weaponMesh->SetStaticMesh(WeaponMeshAsset.Object);
		weaponMesh->SetupAttachment(SphereComponent);
		weaponMesh->SetWorldScale3D(FVector(1.0));
		weaponMesh->SetRelativeRotation(FRotator(0.0, 0.0f, 0.0f));
		weaponMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	
}

void ARifle::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
		Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		
	
	
	
	
}

