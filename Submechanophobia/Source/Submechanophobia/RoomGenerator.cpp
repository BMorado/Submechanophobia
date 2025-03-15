// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGenerator.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoomGenerator::ARoomGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

// Called when the game starts or when spawned
void ARoomGenerator::BeginPlay()
{
	UWorld* World = GetWorld();
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(World,roomSpawnPoints,spawnPointsList);
	Algo::RandomShuffle(spawnPointsList);
	uint8 roomsSpawned = 0;
	while (roomsSpawned < singleRooms.Num())
	{
		AActor* placedRoom = spawnPointsList.Pop();
		spawnedPuzzles.Emplace(World->SpawnActor<APuzzle>(singleRooms[roomsSpawned],placedRoom->GetActorLocation() ,placedRoom->GetTransform().Rotator()));
		roomsSpawned++;
	}
	// After all the puzzle rooms are spawned fill all the leftover spawn points with the default type room
	for (const AActor* list : spawnPointsList)
	{
		FVector TempLocation = list->GetActorLocation();
		FRotator TempRotation = list->GetActorRotation();
		UE_LOG(LogTemp, Log, TEXT("%f"),TempRotation.Yaw);
		
		World->SpawnActor<AActor>(defaultRoom,TempLocation, TempRotation);
	}
	
}

// Called every frame
void ARoomGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

