// Fill out your copyright notice in the Description page of Project Settings.


#include "WallActor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"

// Sets default values
AWallActor::AWallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ProceduralMeshComponent->bUseAsyncCooking = true;
	ProceduralMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay()
{
	Super::BeginPlay();
	
	int32 SectionIndex = 0;
	bool bCreateCollision = true;

	MeshSection Section;
	
	FVector Center(0.0f, 20.0f, 20.0f);
	FVector HalfSize(20.0f, 20.0f, 20.0f);
	FColor Color(255, 255, 255, 255);

	GenerateBoxVertices(Section, Center, HalfSize, Color);
	ProceduralMeshComponent->CreateMeshSection(SectionIndex, 
		Section.Vertices, 
		Section.Triangles,
		Section.Normals,
		Section.UV0,
		Section.VertexColors, 
		Section.Tangents, 
		bCreateCollision);
}

void AWallActor::GenerateBoxVertices(MeshSection& Section, const FVector& Center, const FVector& HalfSize, const FColor& Color)
{
	Section.Vertices =
	{ 
		// -X
		FVector(Center.X - HalfSize.X, Center.Y - HalfSize.Y, Center.Z + HalfSize.Z), 
		FVector(Center.X - HalfSize.X, Center.Y - HalfSize.Y, Center.Z - HalfSize.Z),
		FVector(Center.X - HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X - HalfSize.X, Center.Y + HalfSize.Y, Center.Z - HalfSize.Z),

		// X
		FVector(Center.X + HalfSize.X, Center.Y - HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y - HalfSize.Y, Center.Z - HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z - HalfSize.Z),

		// XY
		FVector(Center.X - HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X - HalfSize.X, Center.Y - HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
	};

	Section.Triangles =
	{
		// -X
		0, 1, 2, 1, 3, 2,

		// X
		4, 5, 6, 5, 7, 6,

		//XY
		8, 9, 10, 9, 11, 10,
	};

	Section.Normals =
	{
		// -X
		FVector(-1.0f, 0.0f, 0.0f), FVector(-1.0f, 0.0f, 0.0f), FVector(-1.0f, 0.0f, 0.0f), FVector(-1.0f, 0.0f, 0.0f),

		// X
		FVector(1.0f, 0.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f),

		//XY
		FVector(0.0f, 0.0f, -1.0f), FVector(0.0f, 0.0f, -1.0f), FVector(0.0f, 0.0f, -1.0f), FVector(0.0f, 0.0f, -1.0f)
	};

	Section.VertexColors.Init(Color, Section.Vertices.Num());

	Section.UV0 = {};
	Section.Tangents = {};
}

// Called every frame
void AWallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

