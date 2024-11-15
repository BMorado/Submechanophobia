// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomGenerator.generated.h"

USTRUCT(BlueprintType)
struct FRoomPair
{
	 GENERATED_BODY()
	 UPROPERTY(EditAnywhere, Category="Rooms")
	 TSubclassOf<AActor> PrimaryRoom;
	
	 UPROPERTY(EditAnywhere, Category="Rooms")
	 TSubclassOf<AActor> PairedRoom;
};





UCLASS()
class SUBMECHANOPHOBIA_API ARoomGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomGenerator();
private:
	uint8 numOfPlayers = 4;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> roomSpawnPoints;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> spawnPointsList;
	UPROPERTY(EditAnywhere, Category="Spawning")
	TArray<FRoomPair> RoomPairs;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> test;
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<AActor> roomsToSpawn;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
