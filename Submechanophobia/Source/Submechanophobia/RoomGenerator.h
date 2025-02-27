// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Puzzle.h"
#include "RoomGenerator.generated.h"

USTRUCT(BlueprintType)
struct FRoomPair
{
	 GENERATED_BODY()
	 UPROPERTY(EditAnywhere, Category="Rooms")
	 TSubclassOf<APuzzle> PrimaryRoom;
	
	 UPROPERTY(EditAnywhere, Category="Rooms")
	 TSubclassOf<APuzzle> SecondaryRoom;
};

UCLASS()
class SUBMECHANOPHOBIA_API ARoomGenerator : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ARoomGenerator();
private:
	// TODO: Get the amount of people playing from the server 
	uint8 numOfPlayers = 4;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// What type of actor to search the world for
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> roomSpawnPoints;

	// List of the room spawn points
	UPROPERTY()
	TArray<AActor*> spawnPointsList;
	
	// Puzzle Rooms that take up two spaces 
	UPROPERTY(EditAnywhere, Category="Spawning")
	TArray<FRoomPair> roomPairs;

	// Puzzle rooms that only take up one space 
	UPROPERTY(EditAnywhere, Category="Spawning")
	TArray<TSubclassOf<APuzzle>>  singleRooms;

	// Room used to fill all empty spaces after all puzzle rooms are spawned 
	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<AActor> defaultRoom;

	// Array of the puzzle that have been spawned
	UPROPERTY()
	// TODO: Swap out AActor* for the actual puzzle type so we can call functions from it  
	TArray<APuzzle*> spawnedPuzzles;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
