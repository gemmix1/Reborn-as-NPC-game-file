#include "GameSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

UGameSettings* UGameSettings::Get()
{
    static UGameSettings* Instance = nullptr;
    if (!Instance)
    {
        Instance = LoadSettings();
        if (!Instance)
        {
            Instance = NewObject<UGameSettings>();
            Instance->ResetToDefaults();
        }
    }
    return Instance;
}

UGameSettings* UGameSettings::LoadSettings()
{
    if (UGameSettings* LoadedSettings = Cast<UGameSettings>(UGameplayStatics::LoadGameFromSlot("GameSettings", 0)))
    {
        return LoadedSettings;
    }
    return nullptr;
}

bool UGameSettings::SaveSettings()
{
    return UGameplayStatics::SaveGameToSlot(this, "GameSettings", 0);
}

void UGameSettings::ResetToDefaults()
{
    DisplaySettings = FDisplaySettings();
    GraphicsSettings = FGraphicsSettings();
    Language = "en";
}

void UGameSettings::ApplySettings()
{
    UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
    if (!UserSettings)
    {
        return;
    }

    // Apply Window Mode
    EWindowMode::Type UEWindowMode;
    switch (DisplaySettings.WindowMode)
    {
        case EWindowMode::Windowed:
            UEWindowMode = EWindowMode::Windowed;
            break;
        case EWindowMode::WindowedFullscreen:
            UEWindowMode = EWindowMode::WindowedFullscreen;
            break;
        case EWindowMode::Fullscreen:
            UEWindowMode = EWindowMode::Fullscreen;
            break;
        default:
            UEWindowMode = EWindowMode::WindowedFullscreen;
            break;
    }
    UserSettings->SetFullscreenMode(UEWindowMode);

    // Apply Resolution
    FIntPoint Resolution(DisplaySettings.ResolutionX, DisplaySettings.ResolutionY);
    UserSettings->SetScreenResolution(Resolution);

    // Apply VSync
    UserSettings->SetVSyncEnabled(DisplaySettings.bVSyncEnabled);

    // Apply Frame Rate Limit
    UserSettings->SetFrameRateLimit(DisplaySettings.FrameRateLimit);

    // Apply Quality Settings
    int32 OverallQuality = static_cast<int32>(GraphicsSettings.OverallQuality);
    UserSettings->SetOverallScalabilityLevel(OverallQuality);

    int32 EffectsQuality = static_cast<int32>(GraphicsSettings.VisualEffectsQuality);
    UserSettings->SetVisualEffectsQuality(EffectsQuality);

    int32 ViewDistanceQuality = static_cast<int32>(GraphicsSettings.ViewDistanceQuality);
    UserSettings->SetViewDistanceQuality(ViewDistanceQuality);

    int32 ShadowQuality = static_cast<int32>(GraphicsSettings.ShadowQuality);
    UserSettings->SetShadowQuality(ShadowQuality);

    int32 TextureQuality = static_cast<int32>(GraphicsSettings.TextureQuality);
    UserSettings->SetTextureQuality(TextureQuality);

    // Apply settings
    UserSettings->ApplySettings(false);
}
