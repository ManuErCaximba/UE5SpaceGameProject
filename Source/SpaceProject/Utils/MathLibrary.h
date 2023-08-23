// MathLibrary.h

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathLibrary.generated.h"

UCLASS()
class SPACEPROJECT_API UMathLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Math")
    static float DotProductZeroAnd360(const FVector Vector1, const FVector Vector2);

    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Math")
    static float TruncateBetweenZeroAnd360(float Direction);

    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Math")
    static float GetDegreesBetweenTwoVectors(FVector VectorA, FVector VectorB);
};