#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpecialAbility.h"
#include "WeaponBase.generated.h"

class ACharacter;

// ================================================================
//  FWeaponStats — UE-seitiger Struct fuer Blueprint-Zugriff
//  Werte werden aus WeaponConfig.h in den Konstruktoren gesetzt
// ================================================================
USTRUCT(BlueprintType)
struct FWeaponStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waffe|Stats")
    float AttackRange = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waffe|Stats")
    int32 Damage = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waffe|Stats")
    float AttackSpeed = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waffe|Stats")
    float Weight = 1.f;
};

// ================================================================
//  AWeaponBase — Abstrakte Basisklasse
//  Pistole / Axt / Schwert erben alle von hier
// ================================================================
UCLASS(Abstract, Blueprintable)
class YOURGAME_API AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    AWeaponBase();

    // --- Mesh ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waffe")
    class UStaticMeshComponent* WaffenMesh;

    // --- Stats ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waffe|Stats")
    FWeaponStats Stats;

    // --- Special Ability ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waffe|Special")
    FSpecialAbility SpecialAbility;

    // ----------------------------------------------------------------
    //  HAUPTFUNKTION
    // ----------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "Waffe")
    virtual void Angreife();

    // --- Equipment ---
    UFUNCTION(BlueprintCallable, Category = "Waffe")
    void OnEquipped(ACharacter* Besitzer);

    UFUNCTION(BlueprintCallable, Category = "Waffe")
    void OnUnequipped();

    // --- Getter ---
    UFUNCTION(BlueprintPure, Category = "Waffe")
    bool KannAngreifen() const { return bKannAngreifen && bIstAusgeruestet; }

    UFUNCTION(BlueprintPure, Category = "Waffe")
    bool IstAusgeruestet() const { return bIstAusgeruestet; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    ACharacter* BesitzerCharakter;

    bool bIstAusgeruestet = false;
    bool bKannAngreifen   = true;

    FTimerHandle AngriffsCooldownTimer;
    FTimerHandle SpecialCooldownTimer;

    void SpecialAbilityAnwenden(AActor* Ziel);
    void CooldownZuruecksetzen();
    void SpecialCooldownZuruecksetzen();
};