// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

ARifle::ARifle()
{
	reserveAmmo = 50;
	
	magazineAmmo = 25;
	
	fireRate = 1.0f;
	
	accuracy = 1.0f;
	
	damage = 8.0f;
	
	isPrimary = true;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARifle::OnOverlapBegin);
}