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
	
	FVector Center(20.0f, 200.0f, 60.0f);
	FVector FrameSize(10.0f, 60.0f, 80.0f);
	float FrameWidth = 2.0f;
	float RailWidth = 2.5f;
	FColor Color(255, 0, 0, 255);

	//GenerateBox(Section, Center, HalfSize, Color);

	GenerateWindow(Section, Center, FrameSize, FrameWidth, RailWidth, Color);
	ProceduralMeshComponent->CreateMeshSection(SectionIndex, 
		Section.Vertices, 
		Section.Triangles,
		Section.Normals,
		Section.UV0,
		Section.VertexColors, 
		Section.Tangents, 
		bCreateCollision);
}

void AWallActor::GenerateBox(MeshSection& Result, const FVector& Center, const FVector& HalfSize, const FColor& Color)
{
	Result.Empty();

	Result.Vertices =
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

		// -Y
		FVector(Center.X - HalfSize.X, Center.Y - HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X - HalfSize.X, Center.Y - HalfSize.Y, Center.Z - HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y - HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y - HalfSize.Y, Center.Z - HalfSize.Z),

		// Y
		FVector(Center.X - HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X - HalfSize.X, Center.Y + HalfSize.Y, Center.Z - HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z - HalfSize.Z),

		// -Z
		FVector(Center.X - HalfSize.X, Center.Y + HalfSize.Y, Center.Z - HalfSize.Z),
		FVector(Center.X - HalfSize.X, Center.Y - HalfSize.Y, Center.Z - HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z - HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y - HalfSize.Y, Center.Z - HalfSize.Z),
		
		// Z
		FVector(Center.X - HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X - HalfSize.X, Center.Y - HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z + HalfSize.Z),
		FVector(Center.X + HalfSize.X, Center.Y - HalfSize.Y, Center.Z + HalfSize.Z)
	};

	Result.Triangles =
	{
		// -X
		0, 1, 2, 1, 3, 2,

		// X
		6, 5, 4, 6, 7, 5,

		// -Y
		10, 9, 8, 10, 11, 9,

		// Y
		12, 13, 14, 13, 15, 14,

		// -Z
		16, 17, 18, 17, 19, 18,

		// Z
		22, 21, 20, 22, 23, 21
	};

	Result.Normals =
	{
		// -X
		FVector(-1.0f, 0.0f, 0.0f), FVector(-1.0f, 0.0f, 0.0f), FVector(-1.0f, 0.0f, 0.0f), FVector(-1.0f, 0.0f, 0.0f),

		// X
		FVector(1.0f, 0.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f),

		// -Y
		FVector(0.0f, -1.0f, 0.0f), FVector(0.0f, -1.0f, 0.0f), FVector(0.0f, -1.0f, 0.0f), FVector(0.0f, -1.0f, 0.0f),

		// Y
		FVector(0.0f, 1.0f, 0.0f), FVector(0.0f, 1.0f, 0.0f), FVector(0.0f, 1.0f, 0.0f), FVector(0.0f, 1.0f, 0.0f),

		// -Z
		FVector(0.0f, 0.0f, -1.0f), FVector(0.0f, 0.0f, -1.0f), FVector(0.0f, 0.0f, -1.0f), FVector(0.0f, 0.0f, -1.0f),

		// Z
		FVector(0.0f, 0.0f, 1.0f), FVector(0.0f, 0.0f, 1.0f), FVector(0.0f, 0.0f, 1.0f), FVector(0.0f, 0.0f, 1.0f)
	};

	Result.VertexColors.Init(Color, Result.Vertices.Num());
}

void AWallActor::GenerateWindow(MeshSection& Result, const FVector& FrameCenter, const FVector& FrameSize, float FrameWidth, float RailWidth, const FColor& Color)
{
	const FVector FrameHalfSize = 0.5f * FrameSize;
	const float FrameHalfWidth = 0.5f * FrameWidth;
	const float RailHalfWidth = 0.5f * RailWidth;

	FVector LeftFramePartCenter(FrameCenter.X, FrameCenter.Y - FrameHalfSize.Y + FrameHalfWidth, FrameCenter.Z);
	FVector RightFramePartCenter(FrameCenter.X, FrameCenter.Y + FrameHalfSize.Y - FrameHalfWidth, FrameCenter.Z);
	FVector LeftFramePartSize(FrameSize.X, FrameWidth, FrameSize.Z - 2.0f * FrameWidth);

	MeshSection LeftFramePart;
	GenerateBox(LeftFramePart, LeftFramePartCenter, 0.5f * LeftFramePartSize, Color);

	MeshSection RightFramePart;
	GenerateBox(RightFramePart, RightFramePartCenter, 0.5f * LeftFramePartSize, Color);

	FVector TopFramePartCenter(FrameCenter.X, FrameCenter.Y, FrameCenter.Z + FrameHalfSize.Z - FrameHalfWidth);
	FVector BottomFramePartCenter(FrameCenter.X, FrameCenter.Y, FrameCenter.Z - FrameHalfSize.Z + FrameHalfWidth);
	FVector TopFramePartSize(FrameSize.X, FrameSize.Y, FrameWidth);

	MeshSection TopFramePart;
	GenerateBox(TopFramePart, TopFramePartCenter, 0.5f * TopFramePartSize, Color);

	MeshSection BottomFramePart;
	GenerateBox(BottomFramePart, BottomFramePartCenter, 0.5f * TopFramePartSize, Color);

	FVector TopRailPartCenter(FrameCenter.X, FrameCenter.Y, FrameCenter.Z + FrameHalfSize.Z - RailHalfWidth);
	FVector TopRailPartSize(FrameSize.X, FrameSize.Y - 2.0f * FrameWidth, FrameSize.Y - 2.0f * RailWidth);

	TArray<MeshSection*> Sections = { &LeftFramePart, &RightFramePart, &TopFramePart, &BottomFramePart};
	MergeSections(Result, Sections);
}

void AWallActor::MergeSections(MeshSection& Result, const TArray<MeshSection*>& SectionsToMerge)
{
	Result.Empty();

	int32 NumVertices = 0;
	int32 NumIndices = 0;
	int32 NumNormals = 0;
	int32 NumUV0 = 0;
	int32 NumVertexColors = 0;
	int32 NumTangents = 0;

	for (const MeshSection* Section : SectionsToMerge)
	{
		NumVertices += Section->Vertices.Num();
		NumIndices += Section->Triangles.Num();
		NumNormals += Section->Normals.Num();
		NumUV0 += Section->UV0.Num();
		NumVertexColors += Section->VertexColors.Num();
		NumTangents += Section->Tangents.Num();
	}

	Result.Vertices.Reserve(NumVertices);
	Result.Triangles.Reserve(NumIndices);
	Result.Normals.Reserve(NumNormals);
	Result.UV0.Reserve(NumUV0);
	Result.VertexColors.Reserve(NumVertexColors);
	Result.Tangents.Reserve(NumTangents);

	for (const MeshSection* Section : SectionsToMerge)
	{
		const int32 VertexOffset = Result.Vertices.Num();
		for (int32 Index : Section->Triangles)
			Result.Triangles.Push(Index + VertexOffset);

		Result.Vertices.Append(Section->Vertices);
		Result.Triangles.Append(Section->Triangles);
		Result.Normals.Append(Section->Normals);
		Result.UV0.Append(Section->UV0);
		Result.VertexColors.Append(Section->VertexColors);
		Result.Tangents.Append(Section->Tangents);
	}
};

// Called every frame
void AWallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

