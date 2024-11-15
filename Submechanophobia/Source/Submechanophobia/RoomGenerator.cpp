// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGenerator.h"
#include <map>
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoomGenerator::ARoomGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ARoomGenerator::BeginPlay()
{
	UWorld* World = GetWorld();
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(World,roomSpawnPoints,spawnPointsList);
	
	
	for(AActor* spawnPoint : spawnPointsList)
	{
		int rand = FMath::RandRange(0,5);
		AActor* temp = World->SpawnActor<AActor>(test[rand],spawnPoint->GetActorLocation(),spawnPoint->GetActorRotation());
	}
	
}

// Called every frame
void ARoomGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

