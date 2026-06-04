#include "WeaponManager.h"
#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

UWeaponManager::UWeaponManager()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponManager::BeginPlay()
{
    Super::BeginPlay();

    BesitzerCharakter = Cast<ACharacter>(GetOwner());

    // StartWaffe automatisch spawnen und ausruesten
    if (StartWaffe && BesitzerCharakter)
    {
        WaffeAusruesten(StartWaffe);
    }
}

// ================================================================
//  Neue Waffe spawnen und ausruesten
// ================================================================
void UWeaponManager::WaffeAusruesten(TSubclassOf<AWeaponBase> WaffenKlasse)
{
    if (!WaffenKlasse || !BesitzerCharakter) return;

    // Alte Waffe erst ablegen
    if (AktuelleWaffe)
        WaffeAblegen();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = BesitzerCharakter;
    SpawnParams.Instigator = BesitzerCharakter;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AWeaponBase* NeueWaffe = GetWorld()->SpawnActor<AWeaponBase>(WaffenKlasse, SpawnParams);
    if (!NeueWaffe) return;

    AktuelleWaffe = NeueWaffe;
    WaffeAnSocketBefestigen(AktuelleWaffe);
    AktuelleWaffe->OnEquipped(BesitzerCharakter);

    OnWaffenWechsel.Broadcast(AktuelleWaffe);
}

// ================================================================
//  Aktuelle Waffe ablegen — aktiviert Physik, Waffe faellt
// ================================================================
void UWeaponManager::WaffeAblegen()
{
    if (!AktuelleWaffe) return;

    AktuelleWaffe->OnUnequipped();
    AktuelleWaffe->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // Physik aktivieren damit die Waffe zu Boden faellt
    UPrimitiveComponent* MeshComponent = nullptr;
    
    if (UStaticMeshComponent* StaticMesh = AktuelleWaffe->FindComponentByClass<UStaticMeshComponent>())
    {
        MeshComponent = StaticMesh;
    }
    else if (USkeletalMeshComponent* SkeletalMesh = AktuelleWaffe->FindComponentByClass<USkeletalMeshComponent>())
    {
        MeshComponent = SkeletalMesh;
    }

    if (MeshComponent)
    {
        MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        MeshComponent->SetSimulatePhysics(true);
    }

    AktuelleWaffe = nullptr;
}

// ================================================================
//  Angriff delegieren
// ================================================================
void UWeaponManager::Angreifen()
{
    if (AktuelleWaffe)
        AktuelleWaffe->Angreife();
}

// ================================================================
//  Waffe an Socket befestigen
// ================================================================
void UWeaponManager::WaffeAnSocketBefestigen(AWeaponBase* Waffe)
{
    if (!Waffe || !BesitzerCharakter) return;

    Waffe->AttachToComponent(
        BesitzerCharakter->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        WaffenSocket
    );
}