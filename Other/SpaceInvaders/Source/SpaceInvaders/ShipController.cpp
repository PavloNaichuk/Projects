// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipController.h"
#include "Components/BoxComponent.h"
#include "BulletController.h"
#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvadersGameMode.h"

// Sets default values
AShipController::AShipController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AShipController::OnOverLap);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AShipController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShipController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero()) 
	{
		FVector NewLocation = GetActorLocation() + Speed * CurrentVelocity * DeltaTime;
		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void AShipController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAxis("MoveX", this, &AShipController::MoveXAxis);
	InputComponent->BindAxis("MoveY", this, &AShipController::MoveYAxis);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &AShipController::OnShoot);
	InputComponent->BindAction("Restart", IE_Pressed, this, &AShipController::OnRestart).bExecuteWhenPaused = true;
}

void AShipController::MoveXAxis(float AxisValue)
{
	CurrentVelocity.X = AxisValue * 100.0f;
}

void AShipController::MoveYAxis(float AxisValue)
{
	CurrentVelocity.Y = AxisValue * 100.0f;
}


void AShipController::OnShoot()
{
	UWorld* World = GetWorld();
	if (World) 
	{
		FVector Location = GetActorLocation();
		World->SpawnActor<ABulletController>(BulletBlueprint, Location, FRotator::ZeroRotator);
	}
}

void AShipController::OnOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEnemyController::StaticClass()))
	{
		Died = true;
		this->SetActorHiddenInGame(true);
		
		((ASpaceInvadersGameMode*)GetWorld()->GetAuthGameMode())->OnGameOver();
		
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AShipController::OnRestart()
{
	if (Died) 
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
}





