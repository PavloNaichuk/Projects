// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCamera.h"

// Sets default values
APawnCamera::APawnCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxModel");
	RootComponent = BoxComponent;

	CameraSpring = CreateDefaultSubobject<USpringArmComponent>("SpringModel");
	CameraSpring->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CameraSpring->AttachTo(BoxComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraModel");
	Camera->AttachTo(CameraSpring, USpringArmComponent::SocketName);

	CameraSpring->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	CameraSpring->TargetArmLength = 1700.0f;
	CameraSpring->bDoCollisionTest = false;

}

// Called when the game starts or when spawned
void APawnCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

