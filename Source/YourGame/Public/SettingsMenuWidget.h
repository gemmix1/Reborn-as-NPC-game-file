#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSettings.h"
#include "SettingsMenuWidget.generated.h"

UCLASS()
class PROJECTILETRAJECTORY_API USettingsMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    /** Initialize widget with current settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void InitializeSettings();

    /** Apply all settings and save */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplyAndSaveSettings();

    /** Reset settings to defaults */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ResetSettings();

    /** Update display settings from UI */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateDisplaySettings(
        EWindowMode WindowMode,
        int32 ResolutionX,
        int32 ResolutionY,
        int32 FrameRateLimit,
        bool bVSyncEnabled,
        bool bHDREnabled
    );

    /** Update graphics settings from UI */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateGraphicsSettings(
        EQualityLevel OverallQuality,
        EQualityLevel VisualEffectsQuality,
        EQualityLevel ViewDistanceQuality,
        EQualityLevel ShadowQuality,
        EQualityLevel TextureQuality
    );

    /** Get current display settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    FDisplaySettings GetDisplaySettings() const;

    /** Get current graphics settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    FGraphicsSettings GetGraphicsSettings() const;

    /** Get available resolutions */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    TArray<FIntPoint> GetAvailableResolutions() const;

    /** Convert quality level to display name */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    static FString GetQualityLevelDisplayName(EQualityLevel Quality);

    /** Convert window mode to display name */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    static FString GetWindowModeDisplayName(EWindowMode WindowMode);

private:
    /** Current game settings */
    UPROPERTY()
    UGameSettings* CurrentSettings;
};
