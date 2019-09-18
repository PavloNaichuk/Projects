// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Snake.h"
#include "SnakeActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "PawnCamera.generated.h"



UCLASS()
class SNAKE_API APawnCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "SnakePawn")
	void AddSnakeToMap();

	void FMove(float Button);

	UFUNCTION(BlueprintCallable, Category = "SnakePawn")
	int32 GetGameMode() const { return GameMode; }

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraSpring;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	ASnakeActor* SnakePlayer;

	FVector2D WSAD;

	int32 GameMode = 0;
};
