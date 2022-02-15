// Fill out your copyright notice in the Description page of Project Settings.

#include "Protagonist.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "ProtagonistPlayerController.h"
#include "ShooterSaveGame.h"
#include "Critter.h"
#include "ItemStorage.h"

AProtagonist::AProtagonist()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (pulls towards the player if there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f; // Camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arm based on controller

	// Set size for collision capsule
	GetCapsuleComponent()->SetCapsuleSize(48.0f, 105.0f);

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match
	// the controller orientation
	FollowCamera->bUsePawnControlRotation = false;
	// Set our turn rates for input
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	// Don't rotate when the controller rotates.
	// Let that just affect the camera.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 650.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	MaxHealth = 100.0f;
	Health = 65.0f;
	MaxStamina = 150.0f;
	Stamina = 120.0f;
	Coins = 0;

	RunningSpeed = 650.0f;
	SprintingSpeed = 950.0f;

	bShiftKeyDown = false;
	bLMBDown = false;

	//Initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.0f;
	MinSprintStamina = 50.0f;

	InterpSpeed = 15.0f;
	bInterpToEnemy = false;

	bHasCombatTarget = false;

	bMovingForward = false;
	bMovingRight = false;

	bESCDown = false;
}

// Called when the game starts or when spawned
void AProtagonist::BeginPlay()
{
	Super::BeginPlay();

	ProtagonistPlayerController = Cast<AProtagonistPlayerController>(GetController());

	LoadGameNoSwitch();
	if (ProtagonistPlayerController)
	{
		ProtagonistPlayerController->GameModeOnly();
	}
}

// Called every frame
void AProtagonist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}
			if (bMovingForward || bMovingRight)
			{
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else
			{
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= 0.0f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				if (bMovingForward || bMovingRight)
				{
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
			}
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.0f;
		}
		else // Shift key up
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}

	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (ProtagonistPlayerController)
		{
			ProtagonistPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

FRotator AProtagonist::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

// Called to bind functionality to input
void AProtagonist::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProtagonist::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AProtagonist::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AProtagonist::ESCUp);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AProtagonist::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AProtagonist::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AProtagonist::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AProtagonist::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProtagonist::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProtagonist::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AProtagonist::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AProtagonist::LookUp);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProtagonist::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProtagonist::LookUpAtRate);

}

bool AProtagonist::AllowInput(float Value)
{
	return (Controller != nullptr) &&
		(Value != 0.0f) &&
		(!bAttacking) &&
		(MovementStatus != EMovementStatus::EMS_Dead) &&
		!ProtagonistPlayerController->bPauseMenuOpen;
}

void AProtagonist::Turn(float Value)
{
	if (AllowInput(Value))
	{
		AddControllerYawInput(Value);
	}
}

void AProtagonist::LookUp(float Value)
{
	if (AllowInput(Value))
	{
		AddControllerPitchInput(Value);
	}
}

void AProtagonist::MoveForward(float Value)
{
	bMovingForward = false;
	if (AllowInput(Value))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		bMovingForward = true;
	}
}


void AProtagonist::MoveRight(float Value)
{
	bMovingRight = false;
	if (AllowInput(Value))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

		bMovingRight = true;
	}
}

void AProtagonist::TurnAtRate(float Rate)
{
	if (AllowInput(Rate))
	{
		float a = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
		float b = FMath::Clamp(a, -1.0f, 1.0f);
		AddControllerYawInput(Rate);
	}
}


void AProtagonist::LookUpAtRate(float Rate)
{
	if (AllowInput(Rate))
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AProtagonist::LMBDown()
{
	bLMBDown = true;

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if (EquippedWeapon)
	{
		Attack();
	}
}

void AProtagonist::LMBUp()
{
	bLMBDown = false;
}

void AProtagonist::ESCDown()
{
	bESCDown = true;

	if (ProtagonistPlayerController)
	{
		ProtagonistPlayerController->TogglePauseMenu();
	}
}

void AProtagonist::ESCUp()
{
	bESCDown = false;
}

void AProtagonist::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.0f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

void AProtagonist::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AProtagonist::IncrementHealth(float Amount)
{
	if (Health + Amount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += Amount;
	}
}

void AProtagonist::Die()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AProtagonist::Jump()
{
	if (ProtagonistPlayerController) if (ProtagonistPlayerController->bPauseMenuOpen) return;
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
}

void AProtagonist::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AProtagonist::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}


void AProtagonist::ShiftKeyDown()
{
	bShiftKeyDown = true;
}


void AProtagonist::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AProtagonist::ShowPickupLocations()
{
	for (auto Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 0.5f);
	}
}

void AProtagonist::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = WeaponToSet;

}

void AProtagonist::Attack()
{
	if (!bAttacking && MovementStatus != EMovementStatus::EMS_Dead)
	{
		bAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;

			default:
				;
			}
		}
	}
}

void AProtagonist::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown)
	{
		Attack();
	}
}

void AProtagonist::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

void AProtagonist::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

float AProtagonist::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();
		if (DamageCauser)
		{
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AProtagonist::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() == 0)
	{
		if (ProtagonistPlayerController)
		{
			ProtagonistPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
		}
		if (ProtagonistPlayerController)
		{
			ProtagonistPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}

void AProtagonist::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();
		CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		FName CurrentLevelName(*CurrentLevel);
		if (CurrentLevelName != LevelName)
		{
			FString Level = LevelName.ToString();
			UE_LOG(LogTemp, Warning, TEXT("CurrentLevel: %s"), *CurrentLevel)
				UE_LOG(LogTemp, Warning, TEXT("LevelName: %s"), *Level)
				UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
}

void AProtagonist::SaveGame()
{
	UShooterSaveGame* SaveObject = Cast<UShooterSaveGame>(UGameplayStatics::CreateSaveGameObject(UShooterSaveGame::StaticClass()));

	SaveObject->CharacterStats.Health = Health;
	SaveObject->CharacterStats.MaxHealth = MaxHealth;
	SaveObject->CharacterStats.Stamina = Stamina;
	SaveObject->CharacterStats.MaxStamina = MaxStamina;
	SaveObject->CharacterStats.Coins = Coins;
	SaveObject->CharacterStats.Location = GetActorLocation();
	SaveObject->CharacterStats.Rotation = GetActorRotation();

	FString MapName = GetWorld()->GetMapName();
	UE_LOG(LogTemp, Warning, TEXT("MapName: %s"), *MapName)
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UE_LOG(LogTemp, Warning, TEXT("MapName: %s"), *MapName)
	SaveObject->CharacterStats.LevelName = MapName;
	UE_LOG(LogTemp, Warning, TEXT("SaveObject->CharacterStats.LevelName: %s"), *SaveObject->CharacterStats.LevelName)
	if (EquippedWeapon)
	{
		SaveObject->CharacterStats.WeaponName = EquippedWeapon->Name;
		SaveObject->CharacterStats.bWeaponParticles = EquippedWeapon->bWeaponParticles;
	}

	UGameplayStatics::SaveGameToSlot(SaveObject, SaveObject->SaveSlotName, SaveObject->UserIndex);
}

void AProtagonist::LoadGame(bool LoadPosition)
{
	UShooterSaveGame* Load = Cast<UShooterSaveGame>(UGameplayStatics::CreateSaveGameObject(UShooterSaveGame::StaticClass()));
	UShooterSaveGame* LoadObject = Cast<UShooterSaveGame>(UGameplayStatics::LoadGameFromSlot(Load->SaveSlotName, Load->UserIndex));

	if (LoadObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level: %s"), *LoadObject->CharacterStats.LevelName)
			if (LoadObject->CharacterStats.LevelName != "")
			{
				FName Map(*LoadObject->CharacterStats.LevelName);
				SwitchLevel(Map);
			}

		Health = LoadObject->CharacterStats.Health;
		MaxHealth = LoadObject->CharacterStats.MaxHealth;

		Stamina = LoadObject->CharacterStats.Stamina;
		MaxStamina = LoadObject->CharacterStats.MaxStamina;

		Coins = LoadObject->CharacterStats.Coins;


		if (WeaponContainer)
		{
			AItemStorage* Container = GetWorld()->SpawnActor<AItemStorage>(WeaponContainer);
			if (Container)
			{
				FString WeaponName = LoadObject->CharacterStats.WeaponName;
				if (Container->WeaponMap.Num() > 0)
				{
					if (Container->WeaponMap.Contains(WeaponName))
					{
						AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Container->WeaponMap[WeaponName]);
						if (Weapon)
						{
							Weapon->bWeaponParticles = LoadObject->CharacterStats.bWeaponParticles;
							Weapon->Equip(this);

						}
					}
				}

			}
		}
		if (LoadPosition)
		{
			SetActorLocation(LoadObject->CharacterStats.Location);
			SetActorRotation(LoadObject->CharacterStats.Rotation);
		}
	}
}

void AProtagonist::LoadGameNoSwitch()
{
	UShooterSaveGame* Load = Cast<UShooterSaveGame>(UGameplayStatics::CreateSaveGameObject(UShooterSaveGame::StaticClass()));
	UShooterSaveGame* LoadObject = Cast<UShooterSaveGame>(UGameplayStatics::LoadGameFromSlot(Load->SaveSlotName, Load->UserIndex));

	if (LoadObject)
	{
		Health = LoadObject->CharacterStats.Health;
		MaxHealth = LoadObject->CharacterStats.MaxHealth;

		Stamina = LoadObject->CharacterStats.Stamina;
		MaxStamina = LoadObject->CharacterStats.MaxStamina;

		Coins = LoadObject->CharacterStats.Coins;


		if (WeaponContainer)
		{
			AItemStorage* Container = GetWorld()->SpawnActor<AItemStorage>(WeaponContainer);
			if (Container)
			{
				FString WeaponName = LoadObject->CharacterStats.WeaponName;
				if (Container->WeaponMap.Num() > 0)
				{
					if (Container->WeaponMap.Contains(WeaponName))
					{
						AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Container->WeaponMap[WeaponName]);
						if (Weapon)
						{
							Weapon->bWeaponParticles = LoadObject->CharacterStats.bWeaponParticles;
							Weapon->Equip(this);

						}
					}
				}

			}
		}
	}
}