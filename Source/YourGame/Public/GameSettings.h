#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSettings.generated.h"

UENUM(BlueprintType)
enum class EWindowMode : uint8
{
    Windowed UMETA(DisplayName = "Windowed"),
    WindowedFullscreen UMETA(DisplayName = "Windowed Fullscreen"),
    Fullscreen UMETA(DisplayName = "Fullscreen")
};

UENUM(BlueprintType)
enum class EQualityLevel : uint8
{
    Low UMETA(DisplayName = "Low"),
    Medium UMETA(DisplayName = "Medium"),
    High UMETA(DisplayName = "High"),
    Epic UMETA(DisplayName = "Epic"),
    Ultra UMETA(DisplayName = "Ultra")
};

USTRUCT(BlueprintType)
struct FDisplaySettings
{
    GENERATED_BODY()

    /** Window mode (Windowed, Windowed Fullscreen, Fullscreen) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
    EWindowMode WindowMode;

    /** Screen resolution width */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
    int32 ResolutionX;

    /** Screen resolution height */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
    int32 ResolutionY;

    /** Frame rate limit (0 = unlimited) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display", meta = (ClampMin = "0"))
    int32 FrameRateLimit;

    /** VSync enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
    bool bVSyncEnabled;

    /** HDR enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
    bool bHDREnabled;

    FDisplaySettings()
        : WindowMode(EWindowMode::WindowedFullscreen)
        , ResolutionX(3840)
        , ResolutionY(1080)
        , FrameRateLimit(30)
        , bVSyncEnabled(true)
        , bHDREnabled(false)
    {}
};

USTRUCT(BlueprintType)
struct FGraphicsSettings
{
    GENERATED_BODY()

    /** Overall quality preset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    EQualityLevel OverallQuality;

    /** Visual effects quality */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    EQualityLevel VisualEffectsQuality;

    /** View distance quality */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    EQualityLevel ViewDistanceQuality;

    /** Shadow quality */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    EQualityLevel ShadowQuality;

    /** Texture quality */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    EQualityLevel TextureQuality;

    FGraphicsSettings()
        : OverallQuality(EQualityLevel::Low)
        , VisualEffectsQuality(EQualityLevel::Low)
        , ViewDistanceQuality(EQualityLevel::Low)
        , ShadowQuality(EQualityLevel::Low)
        , TextureQuality(EQualityLevel::Low)
    {}
};

UCLASS()
class PROJECTILETRAJECTORY_API UGameSettings : public USaveGame
{
    GENERATED_BODY()

public:
    /** Display settings */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FDisplaySettings DisplaySettings;

    /** Graphics settings */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FGraphicsSettings GraphicsSettings;

    /** Language setting */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FString Language;

    UGameSettings()
        : Language("en")
    {}

    /** Apply all settings to the game */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplySettings();

    /** Reset settings to defaults */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ResetToDefaults();

    /** Save settings to disk */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    bool SaveSettings();

    /** Load settings from disk */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    static UGameSettings* LoadSettings();

    /** Get singleton instance */
    static UGameSettings* Get();
};
