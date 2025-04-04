// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"


AUPistol::AUPistol()
{
	reserveAmmo = 10;

	magazineAmmo = 5;

	fireRate = 0.5f;

	accuracy = 1.5f;
	
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
	AltweaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset(TEXT("/Game/Player/SM_Weapons/SM_PISTOL_V1_SA.SM_PISTOL_V1_SA")); 
	
	if (WeaponMeshAsset.Succeeded())
	{
		AltweaponMesh->SetStaticMesh(WeaponMeshAsset.Object);
		AltweaponMesh->SetupAttachment(SphereComponent);
		AltweaponMesh->SetWorldScale3D(FVector(1.0));
		AltweaponMesh->SetRelativeRotation(FRotator(0.0, 0.0f, 0.0f));
		AltweaponMesh->SetRelativeLocation(FVector(23.0f, -10.0f, 0.0f));
	}
}


void AUPistol::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}



