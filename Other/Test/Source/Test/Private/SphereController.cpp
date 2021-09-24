// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereController.h"
#include "Components/SphereComponent.h"

// Sets default values
ASphereController::ASphereController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void ASphereController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASphereController::Tick(float DeltaTime)
{
}

