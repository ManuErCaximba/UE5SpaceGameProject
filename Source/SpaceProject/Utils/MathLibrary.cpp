#include "MathLibrary.h"

float UMathLibrary::DotProductZeroAnd360(const FVector Vector1, const FVector Vector2)
{
    FVector NormalizedVector1 = Vector1.GetSafeNormal();
    FVector NormalizedVector2 = Vector2.GetSafeNormal();
 
    float RadiansAngle = FMath::Atan2(NormalizedVector2.Y, NormalizedVector2.X) - FMath::Atan2(NormalizedVector1.Y, NormalizedVector1.X);

    if (RadiansAngle > PI)
    {
        RadiansAngle -= 2 * PI;
    }
    else if (RadiansAngle < -PI)
    {
        RadiansAngle += 2 * PI;
    }

    float DegreesAngle = FMath::RadiansToDegrees(RadiansAngle);

    return TruncateBetweenZeroAnd360(DegreesAngle);
}

float UMathLibrary::TruncateBetweenZeroAnd360(float Direction)
{
	float Res = Direction;

	if (Direction > 360.f)
	{
		Res = Direction - 360.f;
	}
	else if (Direction < 0.f)
	{
		Res = Direction + 360.f;
	}

	return Res;
}

float UMathLibrary::GetDegreesBetweenTwoVectors(const FVector VectorA, const FVector VectorB)
{
    // Calcula el producto escalar entre los dos vectores
    float DotProduct = FVector::DotProduct(VectorA.GetSafeNormal(), VectorB.GetSafeNormal());

    // Calcula el ángulo en radianes usando la función coseno inverso (arco coseno)
    float AngleRadians = FMath::Acos(DotProduct);

    // Convierte el ángulo de radianes a grados
    return FMath::RadiansToDegrees(AngleRadians);
}