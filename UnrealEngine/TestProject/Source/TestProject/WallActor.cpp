// Fill out your copyright notice in the Description page of Project Settings.


#include "WallActor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Core/Public/Misc/AssertionMacros.h"

// Sets default values
AWallActor::AWallActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("WallMesh");
	ProceduralMeshComponent->bUseAsyncCooking = true;
	ProceduralMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay()
{
	Super::BeginPlay();
	
	int32 SectionIndex = 0;
	bool bCreateCollision = true;

	FVector WallCenter(0.0f, 0.0f, 50.0f);
	FVector WallSize(10.0f, 500.0f, 100.0f);
	FColor WallColor(0, 0, 255, 255);
	int32 NumWindows = 4;
	FVector WindowFrameSize(5.0f, 50.0f, 50.0f);
	float WindowFrameWidth = 5.0f;
	float WindowRailWidth = 4.0f;
	float WindowLeftOffset = 10.0f;
	float WindowTopOffset = 20.0f;
	float DistBetweenWindows = 10.0f;
	FColor WindowColor(255, 255, 0, 255);

	MeshSection Section;
	GenerateWall(Section, WallCenter, WallSize, WallColor,
		NumWindows, WindowFrameSize, WindowFrameWidth, WindowRailWidth,
		WindowLeftOffset, WindowTopOffset, DistBetweenWindows, WindowColor);
	ProceduralMeshComponent->CreateMeshSection(SectionIndex,
		Section.Vertices,
		Section.Triangles,
		Section.Normals,
		Section.UV0,
		Section.VertexColors,
		Section.Tangents,
		bCreateCollision);
}

void AWallActor::GenerateBox(MeshSection& Result, const FVector& Center, const FVector& Size, const FColor& Color)
{
	Result.Empty();
	const FVector HalfSize(0.5f * Size);

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

	TArray<MeshSection> Sections;
	Sections.Reserve(7);

	FVector LeftFramePartCenter(FrameCenter.X, FrameCenter.Y - FrameHalfSize.Y + FrameHalfWidth, FrameCenter.Z);
	FVector RightFramePartCenter(LeftFramePartCenter.X, FrameCenter.Y + FrameHalfSize.Y - FrameHalfWidth, LeftFramePartCenter.Z);
	FVector LeftFramePartSize(FrameSize.X, FrameWidth, FrameSize.Z - 2.0f * FrameWidth);

	Sections.Push(MeshSection());
	GenerateBox(Sections.Last(0), LeftFramePartCenter, LeftFramePartSize, Color);

	Sections.Push(MeshSection());
	GenerateBox(Sections.Last(0), RightFramePartCenter, LeftFramePartSize, Color);

	FVector TopFramePartCenter(FrameCenter.X, FrameCenter.Y, FrameCenter.Z + FrameHalfSize.Z - FrameHalfWidth);
	FVector BottomFramePartCenter(TopFramePartCenter.X, TopFramePartCenter.Y, FrameCenter.Z - FrameHalfSize.Z + FrameHalfWidth);
	FVector TopFramePartSize(FrameSize.X, FrameSize.Y, FrameWidth);

	Sections.Push(MeshSection());
	GenerateBox(Sections.Last(0), TopFramePartCenter, TopFramePartSize, Color);

	Sections.Push(MeshSection());
	GenerateBox(Sections.Last(0), BottomFramePartCenter, TopFramePartSize, Color);

	FVector FrameInnerSize(FrameSize.X, FrameSize.Y - 2.0f * FrameWidth, FrameSize.Z - 2.0f * FrameWidth);
	FVector FrameHalfInnerSize = 0.5f * FrameInnerSize;

	FVector TopRailCenter(FrameCenter.X, FrameCenter.Y, FrameCenter.Z + 0.5f * FrameHalfInnerSize.Z);
	FVector TopRailSize(RailWidth, FrameInnerSize.Y, RailWidth);

	Sections.Push(MeshSection());
	GenerateBox(Sections.Last(0), TopRailCenter, TopRailSize, Color);

	const float RailOffset = FrameInnerSize.Y / 3.0f;
	FVector LeftRailSize(RailWidth, RailWidth, TopRailCenter.Z - 0.5f * RailWidth - (FrameCenter.Z - FrameHalfInnerSize.Z));
	FVector LeftRailCenter(FrameCenter.X, FrameCenter.Y - FrameHalfInnerSize.Y + RailOffset, TopRailCenter.Z - 0.5f * RailWidth - 0.5f * LeftRailSize.Z);
	FVector RightRailCenter(LeftRailCenter.X, FrameCenter.Y + FrameHalfInnerSize.Y - RailOffset, LeftRailCenter.Z);

	Sections.Push(MeshSection());
	GenerateBox(Sections.Last(0), LeftRailCenter, LeftRailSize, Color);

	Sections.Push(MeshSection());
	GenerateBox(Sections.Last(0), RightRailCenter, LeftRailSize, Color);

	MergeSections(Result, Sections);
}

void AWallActor::GenerateWall(MeshSection& Result, const FVector& WallCenter, const FVector& WallSize, const FColor& WallColor,
	int32 NumWindows, const FVector& WindowFrameSize, float WindowFrameWidth, float WindowRailWidth, 
	float WindowLeftOffset, float WindowTopOffset, float DistBetweenWindows, const FColor& WindowColor)
{
	if (NumWindows > 0)
	{
		const FVector WallHalfSize(0.5f * WallSize);
		const FVector WindowFrameHalfSize(0.5f * WindowFrameSize);

		TArray<FVector> WindowCenters;
		WindowCenters.Reserve(NumWindows);

		float WindowYOffset = WallCenter.Y - WallHalfSize.Y + WindowLeftOffset;
		for (int32 Index = 0; Index < NumWindows; ++Index)
		{
			WindowCenters.Emplace(WallCenter.X, WindowYOffset + WindowFrameHalfSize.Y, WallSize.Z - WindowTopOffset - WindowFrameHalfSize.Z);
			WindowYOffset += WindowFrameSize.Y + DistBetweenWindows;
		}

		TArray<MeshSection> Sections;
		Sections.Reserve(2 * NumWindows + 3);

		for (const FVector& WindowCenter : WindowCenters)
		{
			Sections.Push(MeshSection());
			GenerateWindow(Sections.Last(0), WindowCenter, WindowFrameSize, WindowFrameWidth, WindowRailWidth, WindowColor);
		}

		FVector LeftWallPartCenter(WindowCenters[0] - FVector(0.0f, WindowFrameHalfSize.Y + 0.5 * WindowLeftOffset, 0.0f));
		FVector LeftWallPartSize(WallSize.X, WindowLeftOffset, WindowFrameSize.Z);
		Sections.Push(MeshSection());
		GenerateBox(Sections.Last(0), LeftWallPartCenter, LeftWallPartSize, WallColor);

		for (int32 Index = 0; Index < NumWindows - 1; ++Index) 
		{
			FVector WallPartCenter(WindowCenters[Index] + FVector(0.0f, WindowFrameHalfSize.Y + 0.5f * DistBetweenWindows, 0.0f));
			FVector WallPartSize(WallSize.X, DistBetweenWindows, WindowFrameSize.Z);

			Sections.Push(MeshSection());
			GenerateBox(Sections.Last(0), WallPartCenter, WallPartSize, WallColor);
		}

		FVector RightWallPartSize(WallSize.X, WallCenter.Y + WallHalfSize.Y - WindowCenters[NumWindows - 1].Y - WindowFrameHalfSize.Y, WindowFrameSize.Z);
		FVector RightWallPartCenter(WindowCenters[NumWindows - 1] + FVector(0.0f, WindowFrameHalfSize.Y + 0.5f * RightWallPartSize.Y, 0.0f));
		Sections.Push(MeshSection());
		GenerateBox(Sections.Last(0), RightWallPartCenter, RightWallPartSize, WallColor);

		FVector WallTopPartCenter(WallCenter.X, WallCenter.Y, WallCenter.Z + WallHalfSize.Z - 0.5f * WindowTopOffset);
		FVector WallTopPartSize(WallSize.X, WallSize.Y, WindowTopOffset);

		Sections.Push(MeshSection());
		GenerateBox(Sections.Last(0), WallTopPartCenter, WallTopPartSize, WallColor);
		
		float WindowBottomOffset = WallSize.Z - WindowTopOffset - WindowFrameSize.Z;
		FVector WallBottomPartCenter(WallCenter.X, WallCenter.Y, WallCenter.Z - WallHalfSize.Z + 0.5f * WindowBottomOffset);
		FVector WallBottomPartSize(WallSize.X, WallSize.Y, WindowBottomOffset);

		Sections.Push(MeshSection());
		GenerateBox(Sections.Last(0), WallBottomPartCenter, WallBottomPartSize, WallColor);

		MergeSections(Result, Sections);
	}
	else
	{
		GenerateBox(Result, WallCenter, WallSize, WallColor);
	}
}

void AWallActor::MergeSections(MeshSection& Result, const TArray<MeshSection>& Sections)
{
	Result.Empty();

	int32 NumVertices = 0;
	int32 NumIndices = 0;
	int32 NumNormals = 0;
	int32 NumUV0 = 0;
	int32 NumVertexColors = 0;
	int32 NumTangents = 0;

	for (const MeshSection& Section : Sections)
	{
		NumVertices += Section.Vertices.Num();
		NumIndices += Section.Triangles.Num();
		NumNormals += Section.Normals.Num();
		NumUV0 += Section.UV0.Num();
		NumVertexColors += Section.VertexColors.Num();
		NumTangents += Section.Tangents.Num();
	}

	Result.Vertices.Reserve(NumVertices);
	Result.Triangles.Reserve(NumIndices);
	Result.Normals.Reserve(NumNormals);
	Result.UV0.Reserve(NumUV0);
	Result.VertexColors.Reserve(NumVertexColors);
	Result.Tangents.Reserve(NumTangents);

	for (const MeshSection& Section : Sections)
	{
		const int32 VertexOffset = Result.Vertices.Num();
		for (int32 Index : Section.Triangles)
			Result.Triangles.Push(Index + VertexOffset);

		Result.Vertices.Append(Section.Vertices);
		Result.Triangles.Append(Section.Triangles);
		Result.Normals.Append(Section.Normals);
		Result.UV0.Append(Section.UV0);
		Result.VertexColors.Append(Section.VertexColors);
		Result.Tangents.Append(Section.Tangents);
	}
};

// Called every frame
void AWallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallActor::MeshSection::Empty()
{
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UV0.Empty();
	VertexColors.Empty();
	Tangents.Empty();
}