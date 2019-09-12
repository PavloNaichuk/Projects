// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WallActor.generated.h"

UCLASS()
class TESTPROJECT_API AWallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	struct MeshSection
	{
		TArray<FVector> Vertices;
		TArray<int32> Triangles;
		TArray<FVector> Normals;
		TArray<FVector2D> UV0;
		TArray<FColor> VertexColors;
		TArray<FProcMeshTangent> Tangents;

		void Empty();
	};

	static void GenerateBox(MeshSection& Result, const FVector& Center, const FVector& Size, const FColor& Color);

	static void GenerateWindow(MeshSection& Result, const FVector& FrameCenter, const FVector& FrameSize, float FrameWidth, float RailWidth, const FColor& Color);

	static void GenerateWall(MeshSection& Result, const FVector& WallCenter, const FVector& WallSize, const FColor& WallColor,
		int32 NumWindows, const FVector& WindowFrameSize, float WindowFrameWidth, float WindowRailWidth, 
		float WindowLeftOffset, float WindowTopOffset, float DistBetweenWindows, const FColor& WindowColor);

	static void MergeSections(MeshSection& Result, const TArray<MeshSection>& Sections);

private:
	UProceduralMeshComponent* ProceduralMeshComponent;
};
