#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponAxe.generated.h"

// ================================================================
//  AWeaponAxe — Nahkampfwaffe (Axt)
//  Werte kommen aus WeaponConfig.h -> WaffenKonfig::Axt
// ================================================================
UCLASS(Blueprintable)
class YOURGAME_API AWeaponAxe : public AWeaponBase
{
    GENERATED_BODY()

public:
    AWeaponAxe();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axt")
    float SchwingenWinkel = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axt")
    float RueckstossKraft = 700.f;

    virtual void Angreife() override;

private:
    void NahkampfSweep();
    TArray<AActor*> BereitsGetroffen;
};