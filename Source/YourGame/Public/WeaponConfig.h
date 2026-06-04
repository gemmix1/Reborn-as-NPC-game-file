#pragma once

namespace WaffenKonfig
{
    namespace Pistole
    {
        constexpr float AttackRange = 500.f;
        constexpr int   Damage      = 20;
        constexpr float AttackSpeed = 2.5f;
        constexpr float Weight      = 1.0f;

        namespace Special
        {
            constexpr const char* Name             = "Zielfokus";
            constexpr float       DamageMultiplier = 3.0f;
            constexpr float       Radius           = 0.f;
            constexpr float       Cooldown         = 4.0f;
            constexpr const char* StatusEffect     = "None";
            constexpr float       StatusDuration   = 0.f;
        }
    }

    namespace Axt
    {
        constexpr float AttackRange    = 150.f;
        constexpr int   Damage         = 45;
        constexpr float AttackSpeed    = 0.8f;
        constexpr float Weight         = 3.5f;
        constexpr float SchwingenWinkel = 90.f;
        constexpr float RueckstossKraft = 700.f;

        namespace Special
        {
            constexpr const char* Name             = "Wirbelschlag";
            constexpr float       DamageMultiplier = 1.5f;
            constexpr float       Radius           = 280.f;
            constexpr float       Cooldown         = 6.0f;
            constexpr const char* StatusEffect     = "Bleed";
            constexpr float       StatusDuration   = 3.0f;
        }
    }

    namespace Schwert
    {
        constexpr float AttackRange    = 200.f;
        constexpr int   Damage         = 30;
        constexpr float AttackSpeed    = 1.2f;
        constexpr float Weight         = 2.0f;
        constexpr float SchwingenWinkel = 80.f;
        constexpr float RueckstossKraft = 400.f;

        namespace Special
        {
            constexpr const char* Name             = "Sturmhieb";
            constexpr float       DamageMultiplier = 2.0f;
            constexpr float       Radius           = 100.f;
            constexpr float       Cooldown         = 4.0f;
            constexpr const char* StatusEffect     = "Stun";
            constexpr float       StatusDuration   = 1.5f;
        }
    }
}
