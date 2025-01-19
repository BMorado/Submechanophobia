// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGenerator.h"
#include "Algo/RandomShuffle.h"
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
	Algo::RandomShuffle(spawnPointsList);
	
	for (int i=0; i<3;i++)
	{
		spawnedPuzzles.Emplace(World->SpawnActor<APuzzle>(singleRooms[i],spawnPointsList.Pop()->GetActorLocation(),spawnPointsList.Top()->GetActorRotation()));
		//spawnedPuzzles.Emplace(World->SpawnActor<APuzzle>(roomPairs[i].PrimaryRoom,spawnPointsList.Pop()->GetActorLocation(),spawnPointsList.Top()->GetActorRotation()));
		//spawnedPuzzles.Emplace(World->SpawnActor<APuzzle>(roomPairs[i].SecondaryRoom,spawnPointsList.Pop()->GetActorLocation(),spawnPointsList.Top()->GetActorRotation()));
	}

	// After all the puzzle rooms are spawned fill all the leftover spawn points with the default type room
	for (const AActor* list : spawnPointsList)
	{
		World->SpawnActor<AActor>(defaultRoom,list->GetActorLocation(),list->GetActorRotation());
	}
	
}

// Called every frame
void ARoomGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

