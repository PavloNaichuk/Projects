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
	
	//ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("Material'/Game/VertexMat.VertexMat'"));

	int32 SectionIndex = 0;
	TArray<FVector> Vertices = { FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 40.0f), FVector(0.0f, 40.0f, 0.0f) };
	TArray<int32> Triangles = { 0, 2, 1 };
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FColor> VertexColors = { FColor(255, 0, 0, 255), FColor(255, 0, 0, 255), FColor(255, 0, 0, 255) };
	TArray<FProcMeshTangent> Tangents;
	bool bCreateCollision = true;

	ProceduralMeshComponent->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, bCreateCollision);
	//ProceduralMeshComponent->SetMaterial(SectionIndex, MaterialFinder.Object);
}

// Called every frame
void AWallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

