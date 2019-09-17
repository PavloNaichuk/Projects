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
	}
}

