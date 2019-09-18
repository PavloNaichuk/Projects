// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCamera.h"
#include "Components/InputComponent.h"

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

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APawnCamera::BeginPlay()
{
	Super::BeginPlay();
	AddSnakeToMap();
	
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

	InputComponent->BindAxis("KeyMapMove", this, &APawnCamera::FMove);
}

void APawnCamera::AddSnakeToMap()
{
	FVector StartPoint = GetActorLocation();
	FRotator StartPointRotation = GetActorRotation();

	if (GetWorld()) 
	{
		SnakePlayer = GetWorld()->SpawnActor<ASnakeActor>(StartPoint, StartPointRotation);
	}
}

void APawnCamera::FMove(float Button)
{
	int32 Key = Button;
	FVector2D WSAD;
	 switch(Key)
	 {
		case 1:
			if (WSAD.X != 1) 
			{
				WSAD = FVector2D(0, 0);
				WSAD.X = -1;
			}
			
		break;

		case 2:
			if (WSAD.X != -1)
			{
				WSAD = FVector2D(0, 0);
				WSAD.X = 1;
			}
		break;

		case 3:
			if (WSAD.Y != -1)
			{
				WSAD = FVector2D(0, 0);
				WSAD.Y = 1;
			}
		break;

		case 4:
			if (WSAD.Y != -1)
			{
				WSAD = FVector2D(0, 0);
				WSAD.Y = -1;
			}
		break;

	 }
	 if (SnakePlayer != NULL)
	 { 
		 SnakePlayer->DirectionMoveSnake = WSAD; 
	 }
		 
}


