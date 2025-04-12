// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h" 
#include "AmmoPack.generated.h"

UCLASS()
class SUBMECHANOPHOBIA_API AAmmoPack : public AActor
{
	GENERATED_BODY()



	UPROPERTY()
	uint8 ammoYield; 

	UPROPERTY(EditAnywhere,Category = "Weapons")
	UStaticMeshComponent* mesh; 


	UPROPERTY(EditAnywhere,Category = "Weapons")
	TObjectPtr<USphereComponent> SphereComponent;
	
	AAmmoPack();

	UFUNCTION()
	 void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
