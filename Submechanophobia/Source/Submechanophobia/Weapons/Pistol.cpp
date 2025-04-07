// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"


AUPistol::AUPistol()
{
	reserveAmmo = 10;

	magazineAmmo = 5;

	fireRate = 0.05f;

	bulletSpread = FVector(FMath::RandRange(-150.0f, 150.0f),FMath::RandRange(-150.0f, 150.0f),FMath::RandRange(-150.0f, 150.0f));
	
	damage = 15.0f;

	magazineCapacity = 5; 

	isPrimary = false;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AUPistol::OnOverlapBegin);

	SphereComponent->SetRelativeScale3D(FVector(1.0f,1.0f, 1.0f));
	SphereComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	SphereComponent->SetRelativeLocation(FVector(0, 0, 0));

	SphereComponent->SetVisibility(true);

	SphereComponent->ShapeColor = FColor::Green;
	//UStaticMesh*  AltweaponMeshAsset  = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Player/SM_Weapons/railgun.railgun"));
	
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset(TEXT("/Game/Player/Weapons/SM_PISTOL_V1_SA.SM_PISTOL_V1_SA")); 
	
	if (WeaponMeshAsset.Succeeded())
	{
		weaponMesh->SetStaticMesh(WeaponMeshAsset.Object);
		weaponMesh->SetupAttachment(SphereComponent);
		weaponMesh->SetWorldScale3D(FVector(1.0));
		weaponMesh->SetRelativeRotation(FRotator(0.0, 0.0f, 0.0f));
		weaponMesh->SetRelativeLocation(FVector(23.0f, -10.0f, 0.0f));
	}
}


void AUPistol::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}



