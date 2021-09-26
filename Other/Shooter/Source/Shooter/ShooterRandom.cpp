#include "ShooterRandom.h"
#include "Math/UnrealMathUtility.h"

FVector2D ShooterRandom::PointInAnnulus(float InnerRadius, float OuterRadius)
{
	const float U = FMath::RandRange(0.0f, 1.0f);
	const float V = FMath::RandRange(0.0f, 1.0f);

	float Angle = 2.0f * PI * U;
	float Radius = FMath::Sqrt(V * (FMath::Square(OuterRadius) - FMath::Square(InnerRadius)) + FMath::Square(InnerRadius));

	float X, Y;
	FMath::PolarToCartesian(Radius, Angle, X, Y);

	return FVector2D(X, Y);
}