#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BallisticTrajectory.generated.h"

USTRUCT(BlueprintType)
struct FTrajectoryPoint
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory")
    FVector Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory")
    float Time;

    FTrajectoryPoint()
        : Position(FVector::ZeroVector)
        , Time(0.0f)
    {}

    FTrajectoryPoint(FVector InPosition, float InTime)
        : Position(InPosition)
        , Time(InTime)
    {}
};

USTRUCT(BlueprintType)
struct FProjectileData
{
    GENERATED_BODY()

    /** Start position of the projectile */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    FVector StartPosition;

    /** Target landing position */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    FVector TargetPosition;

    /** Initial velocity magnitude (speed) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "0.0"))
    float InitialSpeed;

    /** Gravity override (if 0, uses world gravity) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float GravityOverride;

    /** Number of points to calculate for trajectory */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "2"))
    int32 NumTrajectoryPoints;

    /** Maximum simulation time */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "0.1"))
    float MaxSimulationTime;

    /** Whether to account for air resistance (drag) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    bool bUseAirResistance;

    /** Air resistance coefficient */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "0.0"))
    float DragCoefficient;

    FProjectileData()
        : StartPosition(FVector::ZeroVector)
        , TargetPosition(FVector::ZeroVector)
        , InitialSpeed(1000.0f)
        , GravityOverride(0.0f)
        , NumTrajectoryPoints(50)
        , MaxSimulationTime(5.0f)
        , bUseAirResistance(false)
        , DragCoefficient(0.0f)
    {}
};

UCLASS()
class PROJECTILETRAJECTORY_API UBallisticTrajectory : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Calculates the ballistic trajectory to hit a target position
     * 
     * @param StartPosition - Start position of the projectile
     * @param TargetPosition - Target landing position
     * @param InitialSpeed - Initial velocity magnitude (speed) in cm/s
     * @param GravityOverride - Gravity override (if 0, uses world gravity 980 cm/s^2)
     * @param NumTrajectoryPoints - Number of points to calculate for trajectory
     * @param MaxSimulationTime - Maximum simulation time in seconds
     * @param bUseAirResistance - Whether to account for air resistance (drag)
     * @param DragCoefficient - Air resistance coefficient
     * @param OutTrajectoryPoints - Calculated trajectory points
     * @param OutLandingPosition - Final landing position
     * @param OutLaunchVelocity - Required launch velocity to hit target
     * @param OutFlightTime - Total flight time
     * @return true if calculation was successful, false otherwise
     */
    UFUNCTION(BlueprintCallable, Category = "Ballistic Trajectory")
    static bool CalculateTrajectory(
        FVector StartPosition,
        FVector TargetPosition,
        float InitialSpeed,
        float GravityOverride = 0.0f,
        int32 NumTrajectoryPoints = 50,
        float MaxSimulationTime = 5.0f,
        bool bUseAirResistance = false,
        float DragCoefficient = 0.0f,
        TArray<FTrajectoryPoint>& OutTrajectoryPoints,
        FVector& OutLandingPosition,
        FVector& OutLaunchVelocity,
        float& OutFlightTime
    );

    /**
     * Draws the trajectory as a debug line in the world
     * 
     * @param WorldContext - World context object
     * @param TrajectoryPoints - Trajectory points to draw
     * @param LineColor - Color of the debug line
     * @param LifeTime - How long the line should persist
     * @param Thickness - Line thickness
     */
    UFUNCTION(BlueprintCallable, Category = "Ballistic Trajectory")
    static void DrawTrajectoryDebug(
        UObject* WorldContext,
        const TArray<FTrajectoryPoint>& TrajectoryPoints,
        FLinearColor LineColor,
        float LifeTime,
        float Thickness
    );

    /**
     * Simple trajectory calculation without air resistance
     * 
     * @param StartPos - Starting position
     * @param TargetPos - Target position
     * @param Speed - Initial speed
     * @param OutTrajectory - Output trajectory points
     * @param OutLandingPos - Final landing position
     * @return true if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Ballistic Trajectory")
    static bool CalculateSimpleTrajectory(
        FVector StartPos,
        FVector TargetPos,
        float Speed,
        TArray<FTrajectoryPoint>& OutTrajectory,
        FVector& OutLandingPos
    );

private:
    /** Helper function to solve ballistic equation for launch angle */
    static bool SolveBallisticAngle(
        const FVector& Start,
        const FVector& Target,
        float Speed,
        float Gravity,
        FVector& OutLaunchVelocity
    );

    /** Simulates trajectory with air resistance */
    static void SimulateTrajectoryWithDrag(
        const FVector& Start,
        const FVector& Velocity,
        float Gravity,
        float DragCoeff,
        float MaxTime,
        int32 NumPoints,
        TArray<FTrajectoryPoint>& OutTrajectory
    );

    /** Simulates trajectory without air resistance */
    static void SimulateTrajectoryNoDrag(
        const FVector& Start,
        const FVector& Velocity,
        float Gravity,
        float MaxTime,
        int32 NumPoints,
        TArray<FTrajectoryPoint>& OutTrajectory
    );
};
