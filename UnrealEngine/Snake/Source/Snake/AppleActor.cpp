// Fill out your copyright notice in the Description page of Project Settings.


#include "AppleActor.h"
#include "SnakeActor.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"

// Sets default values
AAppleActor::AAppleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AppleComponent = CreateDefaultSubobject<USphereComponent>("AppleEat");
	RootComponent = AppleComponent;

	SnakeEatMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

	UMaterialInstance* EatColor;
	EatColor = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterial/M_Metal_Gold_Inst.M_Metal_Gold_Inst'")).Object;
	
	FVector Size = FVector(0.5f, 0.5f, 0.5f);
	UStaticMeshComponent* EatChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eat"));

	EatChank->SetStaticMesh(SnakeEatMesh);
	EatChank->SetRelativeScale3D(Size);
	EatChank->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	EatChank->SetMaterial(0, EatColor);
	EatChank->AttachTo(AppleComponent);
	EatChank->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AAppleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAppleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CollecEat();
}

void AAppleActor::CollecEat()
{
	TArray<AActor*> CollectedActors;
	GetOverlappingActors(CollectedActors);

	for (int32 index = 0; index < CollectedActors.Num(); ++index) 
	{
		ASnakeActor* const Test = Cast<ASnakeActor>(CollectedActors[index]);
		if (Test) 
		{
			Test->VisibleBodyChank++;
			Destroy(true, true);
			break;
		}
	}
}

