// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "SnakeActor.generated.h"

UCLASS()
class SNAKE_API ASnakeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreateSnakeBody();
	void SetVisibleChank();
	void MoveSnake();

	int32 SnakeSize = 15;
	float StepSnake = 50.0f;

	UPROPERTY(EditAnywhere)
	int32 VisibleBodyChank = 5;
	
	UPROPERTY(EditAnywhere)
	FVector2D DirectionMoveSnake;
	
	float StepDelay = 0.5f;
	float BufferTime = 0;

	TArray<UStaticMeshComponent*> SnakeBody;
	USphereComponent* SphereComponent;
};
