// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpline.h"

#include "Components/SplineComponent.h"

// Sets default values
AEnemySpline::AEnemySpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
}

void AEnemySpline::GetSplinePointAsWorldPos(FVector& Location)
{
	Location = Spline->GetLocationAtSplinePoint(patrolIndex,ESplineCoordinateSpace::World); 
}

// Called when the game starts or when spawned
void AEnemySpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpline::IncrementPatrolRoute()
{
	patrolIndex += direction;
	if (patrolIndex == (Spline->GetNumberOfSplinePoints() -1 ))
	{
		direction = -1;
	}
	else if (patrolIndex == 0)
	{
		direction = 1;
	}
}

