// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterSphere.h"
#include "Components/SphereComponent.h"
#include "ShooterGameMode.h"

// Sets default values
AShooterSphere::AShooterSphere()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
}

// Called when the game starts or when spawned
void AShooterSphere::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AShooterSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShooterSphere::OnHit()
{
	AShooterGameMode* GameMode = (AShooterGameMode*)GetWorld()->GetAuthGameMode();
	GameMode->OnSphereHit(this);

	Destroy();
}