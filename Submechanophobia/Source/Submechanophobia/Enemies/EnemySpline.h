// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "EnemySpline.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIncrementPatrolRoute);
UCLASS()
class SUBMECHANOPHOBIA_API AEnemySpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpline();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	virtual void GetSplinePointAsWorldPos(FVector& Location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USplineComponent> Spline;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(ClampMin="-1", ClampMax="1"))
	int32 patrolIndex;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	int32 direction;

	UPROPERTY()
	FIncrementPatrolRoute OnIncrementPatrolRoute;



};
