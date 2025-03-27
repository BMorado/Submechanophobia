// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "Submechanophobia/Player/APlayerCharacter.h"

ARifle::ARifle()
{
	reserveAmmo = 50;
	
	magazineAmmo = 25;
	
	fireRate = 1.0f;
	
	accuracy = 1.0f;
	
	damage = 8.0f;
	
	isPrimary = true;

	

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARifle::OnOverlapBegin);

	SphereComponent->SetRelativeScale3D(FVector(1.0f,1.0f, 1.0f));
	SphereComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	SphereComponent->SetRelativeLocation(FVector(0, 0, 0));

	
	SphereComponent->SetVisibility(true);
	SphereComponent->ShapeColor = FColor::Green;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshAsset(TEXT("/Game/Boss_assets/Scuttling_Husk/scuttling_husk_exp_feb2.scuttling_husk_exp_feb2"));
	if (WeaponMeshAsset.Succeeded())
	{
		weaponMesh->SetSkeletalMesh(WeaponMeshAsset.Object);
		weaponMesh->SetupAttachment(SphereComponent);
		weaponMesh->SetWorldScale3D(FVector(9.0));
		weaponMesh->SetRelativeRotation(FRotator(-90.0, -90.0f, 0.0f));
		weaponMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	
}

void ARifle::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	
}

