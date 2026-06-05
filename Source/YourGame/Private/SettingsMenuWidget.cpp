#include "SettingsMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

void USettingsMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    InitializeSettings();
}

void USettingsMenuWidget::InitializeSettings()
{
    CurrentSettings = UGameSettings::Get();
    if (!CurrentSettings)
    {
        CurrentSettings = NewObject<UGameSettings>(this);
        CurrentSettings->ResetToDefaults();
    }
}

void USettingsMenuWidget::ApplyAndSaveSettings()
{
    if (CurrentSettings)
    {
        CurrentSettings->ApplySettings();
        CurrentSettings->SaveSettings();
    }
}

void USettingsMenuWidget::ResetSettings()
{
    if (CurrentSettings)
    {
        CurrentSettings->ResetToDefaults();
    }
}

void USettingsMenuWidget::UpdateDisplaySettings(
    EWindowMode WindowMode,
    int32 ResolutionX,
    int32 ResolutionY,
    int32 FrameRateLimit,
    bool bVSyncEnabled,
    bool bHDREnabled)
{
    if (CurrentSettings)
    {
        CurrentSettings->DisplaySettings.WindowMode = WindowMode;
        CurrentSettings->DisplaySettings.ResolutionX = ResolutionX;
        CurrentSettings->DisplaySettings.ResolutionY = ResolutionY;
        CurrentSettings->DisplaySettings.FrameRateLimit = FrameRateLimit;
        CurrentSettings->DisplaySettings.bVSyncEnabled = bVSyncEnabled;
        CurrentSettings->DisplaySettings.bHDREnabled = bHDREnabled;
    }
}

void USettingsMenuWidget::UpdateGraphicsSettings(
    EQualityLevel OverallQuality,
    EQualityLevel VisualEffectsQuality,
    EQualityLevel ViewDistanceQuality,
    EQualityLevel ShadowQuality,
    EQualityLevel TextureQuality)
{
    if (CurrentSettings)
    {
        CurrentSettings->GraphicsSettings.OverallQuality = OverallQuality;
        CurrentSettings->GraphicsSettings.VisualEffectsQuality = VisualEffectsQuality;
        CurrentSettings->GraphicsSettings.ViewDistanceQuality = ViewDistanceQuality;
        CurrentSettings->GraphicsSettings.ShadowQuality = ShadowQuality;
        CurrentSettings->GraphicsSettings.TextureQuality = TextureQuality;
    }
}

FDisplaySettings USettingsMenuWidget::GetDisplaySettings() const
{
    if (CurrentSettings)
    {
        return CurrentSettings->DisplaySettings;
    }
    return FDisplaySettings();
}

FGraphicsSettings USettingsMenuWidget::GetGraphicsSettings() const
{
    if (CurrentSettings)
    {
        return CurrentSettings->GraphicsSettings;
    }
    return FGraphicsSettings();
}

TArray<FIntPoint> USettingsMenuWidget::GetAvailableResolutions() const
{
    TArray<FIntPoint> Resolutions;
    
    UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
    if (UserSettings)
    {
        Resolutions = UserSettings->GetSupportedScreenResolutions();
    }
    
    // Add common resolutions if none found
    if (Resolutions.Num() == 0)
    {
        Resolutions.Add(FIntPoint(1920, 1080));
        Resolutions.Add(FIntPoint(2560, 1440));
        Resolutions.Add(FIntPoint(3840, 2160));
        Resolutions.Add(FIntPoint(3840, 1080));
    }
    
    return Resolutions;
}

FString USettingsMenuWidget::GetQualityLevelDisplayName(EQualityLevel Quality)
{
    switch (Quality)
    {
        case EQualityLevel::Low:
            return TEXT("Low");
        case EQualityLevel::Medium:
            return TEXT("Medium");
        case EQualityLevel::High:
            return TEXT("High");
        case EQualityLevel::Epic:
            return TEXT("Epic");
        case EQualityLevel::Ultra:
            return TEXT("Ultra");
        default:
            return TEXT("Unknown");
    }
}

FString USettingsMenuWidget::GetWindowModeDisplayName(EWindowMode WindowMode)
{
    switch (WindowMode)
    {
        case EWindowMode::Windowed:
            return TEXT("Windowed");
        case EWindowMode::WindowedFullscreen:
            return TEXT("Windowed Fullscreen");
        case EWindowMode::Fullscreen:
            return TEXT("Fullscreen");
        default:
            return TEXT("Unknown");
    }
}
