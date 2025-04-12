// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPack.h"

#include "Components/SphereComponent.h"
#include "Submechanophobia/Player/APlayerCharacter.h"

// Sets default values
AAmmoPack::AAmmoPack()
{

	ammoYield = 30;


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo Mesh")); 


	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetVisibility(true);
	SphereComponent->ShapeColor = FColor::Green;
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPack::OnOverlapBegin);
	SphereComponent->SetRelativeScale3D(FVector(1.0f,1.0f, 1.0f));
	SphereComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	SphereComponent->SetRelativeLocation(FVector(0, 0, 0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Player/Weapons/SM_Bullet.SM_Bullet")); 

	if (MeshAsset.Succeeded())
	{
		mesh->SetStaticMesh(MeshAsset.Object);
		mesh->SetupAttachment(SphereComponent);
		mesh->SetWorldScale3D(FVector(1.0));
		mesh->SetRelativeRotation(FRotator(0.0, 0.0f, 0.0f));
		mesh->SetRelativeLocation(FVector(-15.0f, -15.0f, 0.0f));
	}

	
	

}

void AAmmoPack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AAPlayerCharacter* temp = dynamic_cast<AAPlayerCharacter*>(OtherActor);
	if (temp != nullptr && temp->GetCurrentWeapon() != nullptr)
	{
		temp->GetCurrentWeapon()->reserveAmmo  +=  ammoYield;

		this->Destroy();
		
	}
	
}


