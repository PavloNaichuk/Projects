// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtagonistAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Protagonist.h"

void UProtagonistAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Protagonist = Cast<AProtagonist>(Pawn);
		}
	}


}

void UProtagonistAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		MovementSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
		if (Protagonist == nullptr)
		{
			Protagonist = Cast<AProtagonist>(Pawn);
		}
	}
}
