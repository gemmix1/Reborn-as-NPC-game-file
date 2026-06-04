#pragma once

#include "CoreMinimal.h"
#include "SpecialAbility.generated.h"

USTRUCT(BlueprintType)
struct FSpecialAbility
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAbility")
    FString AbilityName = "Default";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAbility")
    float DamageMultiplier = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAbility")
    float SpecialRadius = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAbility")
    float Cooldown = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAbility")
    FString StatusEffect = "None";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAbility")
    float StatusEffectDuration = 2.f;

    UPROPERTY(BlueprintReadOnly, Category = "SpecialAbility")
    bool bIsReady = true;
};