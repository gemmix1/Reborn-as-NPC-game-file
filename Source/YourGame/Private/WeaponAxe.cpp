#include "WeaponAxe.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

AWeaponAxe::AWeaponAxe()
{
    Stats.AttackRange = 150.f;
    Stats.Damage = 45;
    Stats.AttackSpeed = 0.8f;
    Stats.Weight = 3.5f;

    SpecialAbility.AbilityName = "Wirbelschlag";
    SpecialAbility.DamageMultiplier = 1.5f;
    SpecialAbility.SpecialRadius = 280.f;
    SpecialAbility.Cooldown = 6.0f;
    SpecialAbility.StatusEffect = "Bleed";
    SpecialAbility.StatusEffectDuration = 3.0f;
}

void AWeaponAxe::Angreife()
{
    Super::Angreife();
    NahkampfSweep();
}

void AWeaponAxe::NahkampfSweep()
{
    BereitsGetroffen.Empty();
    UE_LOG(LogTemp, Warning, TEXT("Axt-Angriff! Winkel: %.1f Grad"), SchwingenWinkel);
}