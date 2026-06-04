#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;

    WaffenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaffenMesh"));
    RootComponent = WaffenMesh;

    WaffenMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
}

void AWeaponBase::Angreife()
{
    if (!KannAngreifen()) return;

    UE_LOG(LogTemp, Warning, TEXT("Waffe '%s' greift an!"), *GetName());

    bKannAngreifen = false;

    GetWorld()->GetTimerManager().SetTimer(
        AngriffsCooldownTimer,
        this,
        &AWeaponBase::CooldownZuruecksetzen,
        1.0f / Stats.AttackSpeed,
        false
    );
}

void AWeaponBase::OnEquipped(ACharacter* Besitzer)
{
    if (!Besitzer) return;

    BesitzerCharakter = Besitzer;
    bIstAusgeruestet = true;

    WaffenMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WaffenMesh->SetSimulatePhysics(false);

    UE_LOG(LogTemp, Warning, TEXT("Waffe '%s' ausgeruestet!"), *GetName());
}

void AWeaponBase::OnUnequipped()
{
    bIstAusgeruestet = false;
    BesitzerCharakter = nullptr;

    UE_LOG(LogTemp, Warning, TEXT("Waffe '%s' abgelegt!"), *GetName());
}

void AWeaponBase::SpecialAbilityAnwenden(AActor* Ziel)
{
    if (!Ziel || !SpecialAbility.bIsReady) return;

    float Schaden = Stats.Damage * SpecialAbility.DamageMultiplier;

    UGameplayStatics::ApplyRadialDamage(
        GetWorld(),
        Schaden,
        Ziel->GetActorLocation(),
        SpecialAbility.SpecialRadius,
        UDamageType::StaticClass(),
        TArray<AActor*>(),
        this,
        nullptr,
        true
    );

    SpecialAbility.bIsReady = false;

    GetWorld()->GetTimerManager().SetTimer(
        SpecialCooldownTimer,
        this,
        &AWeaponBase::SpecialCooldownZuruecksetzen,
        SpecialAbility.Cooldown,
        false
    );

    UE_LOG(LogTemp, Warning, TEXT("SpecialAbility '%s' ausgeloest!"), *SpecialAbility.AbilityName);
}

void AWeaponBase::CooldownZuruecksetzen()
{
    bKannAngreifen = true;
}

void AWeaponBase::SpecialCooldownZuruecksetzen()
{
    SpecialAbility.bIsReady = true;
    UE_LOG(LogTemp, Warning, TEXT("SpecialAbility '%s' bereit!"), *SpecialAbility.AbilityName);
}