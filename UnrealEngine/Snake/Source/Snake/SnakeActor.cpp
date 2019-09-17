// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"

// Sets default values
ASnakeActor::ASnakeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	RootComponent = SphereComponent;
	FVector Position = GetActorLocation();

	SphereComponent->SetRelativeLocation(Position);
	CreateSnakeBody();
}

// Called when the game starts or when spawned
void ASnakeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetVisibleChank();
	BufferTime += DeltaTime;
	if (BufferTime > StepDelay)
	{
		MoveSnake();
		BufferTime = 0;
	}
}

void ASnakeActor::CreateSnakeBody()
{
	UStaticMesh* SnakeChankMesh;
	SnakeChankMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

	UMaterialInstance* BodyColor;
	BodyColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterial/M_Tech_Hex_Tile_2_Inst.M_Tech_Hex_Tile_2_Inst'")).Get();
	
	UMaterialInstance* WormHead;
	WormHead = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterial/M_Tech_Hex_Tile_Pulse_Inst.M_Tech_Hex_Tile_Pulse_Inst'")).Get();

	FName NameChank;
	FString TheString;
	FVector NextPoint = GetActorLocation();

	for (int32 index = 0; index < SnakeSize; ++index)
	{
		TheString = "Shanks" + FString::FromInt(index);
		NameChank = FName(*TheString);
		UStaticMeshComponent* BodyChank = CreateDefaultSubobject<UStaticMeshComponent>(NameChank);
		BodyChank->SetStaticMesh(SnakeChankMesh);

		BodyChank->SetRelativeLocation(NextPoint);
		SnakeBody.Add(BodyChank);

		NextPoint.X -= StepSnake;
		BodyChank->AttachTo(SphereComponent);

		BodyChank->SetMaterial(0, BodyColor);

		if (index == 0)
		{
			BodyChank->SetMaterial(0, WormHead);
		}
		else
		{
			BodyChank->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	SetVisibleChank();
}

void ASnakeActor::SetVisibleChank()
{
	for (int32 Index = 0; Index < SnakeBody.Num(); ++Index) 
	{
		if (Index < VisibleBodyChank)
		{
			SnakeBody[Index]->SetVisibility(true, true);
		}
		else
		{
			SnakeBody[Index]->SetVisibility(false, true);
		}
	}
}

void ASnakeActor::MoveSnake()
{
	if ((DirectionMoveSnake.X != 0) || (DirectionMoveSnake.Y != 0)) 
	{
		for (int Chank = SnakeBody.Num() - 1; Chank > 0; --Chank)
		{
			FVector V = SnakeBody[Chank - 1]->RelativeLocation;
			SnakeBody[Chank]->SetRelativeLocation(V);
		}
		FVector StartPoint = SnakeBody[0]->RelativeLocation;
		if (DirectionMoveSnake.X > 0)
			StartPoint.X -= StepSnake;
		if (DirectionMoveSnake.X < 0)
			StartPoint.X += StepSnake;

		if (DirectionMoveSnake.Y > 0)
			StartPoint.X += StepSnake;
		if (DirectionMoveSnake.Y < 0)
			StartPoint.X -= StepSnake;
		SnakeBody[0]->SetRelativeLocation(StartPoint);
	}
}

