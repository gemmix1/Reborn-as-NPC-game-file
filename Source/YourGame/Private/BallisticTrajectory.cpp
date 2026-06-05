#include "BallisticTrajectory.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

bool UBallisticTrajectory::CalculateTrajectory(
    FVector StartPosition,
    FVector TargetPosition,
    float InitialSpeed,
    float GravityOverride,
    int32 NumTrajectoryPoints,
    float MaxSimulationTime,
    bool bUseAirResistance,
    float DragCoefficient,
    TArray<FTrajectoryPoint>& OutTrajectoryPoints,
    FVector& OutLandingPosition,
    FVector& OutLaunchVelocity,
    float& OutFlightTime)
{
    OutTrajectoryPoints.Empty();
    OutLandingPosition = FVector::ZeroVector;
    OutLaunchVelocity = FVector::ZeroVector;
    OutFlightTime = 0.0f;

    // Get gravity (use override if provided, otherwise use world gravity)
    float Gravity = GravityOverride;
    if (Gravity <= 0.0f)
    {
        Gravity = 980.0f; // Default UE5 gravity (cm/s^2)
    }

    // Calculate required launch velocity to hit target
    if (!SolveBallisticAngle(
        StartPosition,
        TargetPosition,
        InitialSpeed,
        Gravity,
        OutLaunchVelocity))
    {
        return false;
    }

    // Simulate trajectory
    if (bUseAirResistance)
    {
        SimulateTrajectoryWithDrag(
            StartPosition,
            OutLaunchVelocity,
            Gravity,
            DragCoefficient,
            MaxSimulationTime,
            NumTrajectoryPoints,
            OutTrajectoryPoints);
    }
    else
    {
        SimulateTrajectoryNoDrag(
            StartPosition,
            OutLaunchVelocity,
            Gravity,
            MaxSimulationTime,
            NumTrajectoryPoints,
            OutTrajectoryPoints);
    }

    // Set output values
    if (OutTrajectoryPoints.Num() > 0)
    {
        OutLandingPosition = OutTrajectoryPoints.Last().Position;
        OutFlightTime = OutTrajectoryPoints.Last().Time;
    }

    return true;
}

void UBallisticTrajectory::DrawTrajectoryDebug(
    UObject* WorldContext,
    const TArray<FTrajectoryPoint>& TrajectoryPoints,
    FLinearColor LineColor,
    float LifeTime,
    float Thickness)
{
    if (!WorldContext || TrajectoryPoints.Num() < 2)
    {
        return;
    }

    UWorld* World = WorldContext->GetWorld();
    if (!World)
    {
        return;
    }

    for (int32 i = 0; i < TrajectoryPoints.Num() - 1; i++)
    {
        const FTrajectoryPoint& PointA = TrajectoryPoints[i];
        const FTrajectoryPoint& PointB = TrajectoryPoints[i + 1];

        DrawDebugLine(
            World,
            PointA.Position,
            PointB.Position,
            LineColor.ToFColor(true),
            false,
            LifeTime,
            0,
            Thickness
        );
    }

    // Draw start and end points
    if (TrajectoryPoints.Num() > 0)
    {
        DrawDebugPoint(
            World,
            TrajectoryPoints[0].Position,
            10.0f,
            FColor::Green,
            false,
            LifeTime
        );

        DrawDebugPoint(
            World,
            TrajectoryPoints.Last().Position,
            10.0f,
            FColor::Red,
            false,
            LifeTime
        );
    }
}

bool UBallisticTrajectory::CalculateSimpleTrajectory(
    FVector StartPos,
    FVector TargetPos,
    float Speed,
    TArray<FTrajectoryPoint>& OutTrajectory,
    FVector& OutLandingPos)
{
    OutTrajectory.Empty();
    OutLandingPos = FVector::ZeroVector;

    FVector LaunchVelocity;
    float FlightTime;

    return CalculateTrajectory(
        StartPos,
        TargetPos,
        Speed,
        0.0f,           // GravityOverride (use default)
        50,             // NumTrajectoryPoints
        5.0f,           // MaxSimulationTime
        false,          // bUseAirResistance
        0.0f,           // DragCoefficient
        OutTrajectory,
        OutLandingPos,
        LaunchVelocity,
        FlightTime
    );
}

bool UBallisticTrajectory::SolveBallisticAngle(
    const FVector& Start,
    const FVector& Target,
    float Speed,
    float Gravity,
    FVector& OutLaunchVelocity)
{
    FVector Delta = Target - Start;
    float HorizontalDistance = FMath::Sqrt(Delta.X * Delta.X + Delta.Y * Delta.Y);
    float VerticalDistance = Delta.Z;

    // Check if target is reachable
    float SpeedSquared = Speed * Speed;
    float SpeedFourth = SpeedSquared * SpeedSquared;
    
    // Discriminant for quadratic equation
    float Discriminant = SpeedFourth - Gravity * (Gravity * HorizontalDistance * HorizontalDistance + 2.0f * VerticalDistance * SpeedSquared);
    
    if (Discriminant < 0.0f)
    {
        // Target is out of range
        return false;
    }

    // Calculate launch angle (use higher arc for better visibility)
    float Angle = FMath::Atan((SpeedSquared + FMath::Sqrt(Discriminant)) / (Gravity * HorizontalDistance));

    // Calculate launch velocity components
    float Vx = Speed * FMath::Cos(Angle) * (Delta.X / HorizontalDistance);
    float Vy = Speed * FMath::Cos(Angle) * (Delta.Y / HorizontalDistance);
    float Vz = Speed * FMath::Sin(Angle);

    OutLaunchVelocity = FVector(Vx, Vy, Vz);
    return true;
}

void UBallisticTrajectory::SimulateTrajectoryWithDrag(
    const FVector& Start,
    const FVector& Velocity,
    float Gravity,
    float DragCoeff,
    float MaxTime,
    int32 NumPoints,
    TArray<FTrajectoryPoint>& OutTrajectory)
{
    OutTrajectory.Empty();
    
    FVector CurrentPos = Start;
    FVector CurrentVel = Velocity;
    float TimeStep = MaxTime / (NumPoints - 1);
    float CurrentTime = 0.0f;

    for (int32 i = 0; i < NumPoints; i++)
    {
        OutTrajectory.Add(FTrajectoryPoint(CurrentPos, CurrentTime));

        // Apply drag force: F_drag = -0.5 * rho * Cd * A * v^2
        // Simplified: a_drag = -DragCoeff * v * |v|
        float Speed = CurrentVel.Size();
        if (Speed > 0.0f)
        {
            FVector DragAcceleration = -DragCoeff * CurrentVel * Speed;
            CurrentVel += (DragAcceleration + FVector(0.0f, 0.0f, -Gravity)) * TimeStep;
        }
        else
        {
            CurrentVel.Z -= Gravity * TimeStep;
        }

        CurrentPos += CurrentVel * TimeStep;
        CurrentTime += TimeStep;
    }
}

void UBallisticTrajectory::SimulateTrajectoryNoDrag(
    const FVector& Start,
    const FVector& Velocity,
    float Gravity,
    float MaxTime,
    int32 NumPoints,
    TArray<FTrajectoryPoint>& OutTrajectory)
{
    OutTrajectory.Empty();
    
    float TimeStep = MaxTime / (NumPoints - 1);

    for (int32 i = 0; i < NumPoints; i++)
    {
        float Time = i * TimeStep;
        
        // Physics formula: P(t) = P0 + v0*t + 0.5*g*t^2
        FVector Position = Start + Velocity * Time;
        Position.Z -= 0.5f * Gravity * Time * Time;

        OutTrajectory.Add(FTrajectoryPoint(Position, Time));
    }
}
