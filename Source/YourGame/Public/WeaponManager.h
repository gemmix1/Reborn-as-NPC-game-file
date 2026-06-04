#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManager.generated.h"

class AWeaponBase;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaffenWechsel, AWeaponBase*, NeueWaffe);

// ================================================================
//  UWeaponManager — ActorComponent
//  Zum Charakter hinzufuegen, StartWaffe im Blueprint setzen.
//  Kuemmert sich um: Ausruestemen, Ablegen, Wechseln, Angreifen.
// ================================================================
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YOURGAME_API UWeaponManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponManager();

    // Socket-Name am Charakter-Skelett (im Blueprint/Editor setzen)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaffenManager")
    FName WaffenSocket = "WeaponSocket";

    // Waffe die beim Spielstart automatisch gespawnt wird
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaffenManager")
    TSubclassOf<AWeaponBase> StartWaffe;

    // Wird gefeuert wenn eine neue Waffe ausgeruestet wird
    UPROPERTY(BlueprintAssignable, Category = "WaffenManager|Events")
    FOnWaffenWechsel OnWaffenWechsel;

    // ----------------------------------------------------------------
    //  Oeffentliche Funktionen
    // ----------------------------------------------------------------

    // Neue Waffe nach Klasse spawnen und ausruesten
    UFUNCTION(BlueprintCallable, Category = "WaffenManager")
    void WaffeAusruesten(TSubclassOf<AWeaponBase> WaffenKlasse);

    // Aktuelle Waffe ablegen (faellt zu Boden)
    UFUNCTION(BlueprintCallable, Category = "WaffenManager")
    void WaffeAblegen();

    // Angriff mit der aktuellen Waffe ausfuehren
    UFUNCTION(BlueprintCallable, Category = "WaffenManager")
    void Angreifen();

    // --- Getter ---
    UFUNCTION(BlueprintPure, Category = "WaffenManager")
    AWeaponBase* GetAktuelleWaffe() const { return AktuelleWaffe; }

    UFUNCTION(BlueprintPure, Category = "WaffenManager")
    bool HatWaffe() const { return AktuelleWaffe != nullptr; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    AWeaponBase* AktuelleWaffe = nullptr;

    UPROPERTY()
    ACharacter* BesitzerCharakter = nullptr;

    void WaffeAnSocketBefestigen(AWeaponBase* Waffe);
};