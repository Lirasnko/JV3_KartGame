// Fill out your copyright notice in the Description page of Project Settings.

#include "KartPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "KartGameMode.h"
#include "Sound/SoundWaveProxyPlayer.h"

static constexpr float GroundSnapTolerance = 5.f;

static constexpr float WalkableFloorZ = 0.7f;
// Sets default values
AKartPawn::AKartPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(35.f);
	CollisionSphere->SetCollisionProfileName(TEXT("Pawn"));
	CollisionSphere->SetSimulatePhysics(false);
	RootComponent = CollisionSphere;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(CollisionSphere);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WheelFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFrontLeft"));
	WheelFrontLeft->SetupAttachment(BodyMesh);
	WheelFrontLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WheelFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFrontRight"));
	WheelFrontRight->SetupAttachment(BodyMesh);
	WheelFrontRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WheelRearLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelRearLeft"));
	WheelRearLeft->SetupAttachment(BodyMesh);
	WheelRearLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WheelRearRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelRearRight"));
	WheelRearRight->SetupAttachment(BodyMesh);
	WheelRearRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CollisionSphere);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 6.f;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritPitch = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void AKartPawn::BeginPlay()
{
	Super::BeginPlay();
	
	BodyBaseRotation = BodyMesh->GetRelativeRotation();
	SpringArmBaseRotation = SpringArm->GetRelativeRotation();
	WheelFrontLeftBaseRotation = WheelFrontLeft->GetRelativeRotation();
	WheelFrontRightBaseRotation = WheelFrontRight->GetRelativeRotation();
	WheelRearLeftBaseRotation = WheelRearLeft->GetRelativeRotation();
	WheelRearRightBaseRotation = WheelRearRight->GetRelativeRotation();
	
	if (const AKartGameMode* GameMode = GetWorld()->GetAuthGameMode<AKartGameMode>())
	{
		bCanMove = GameMode->IsRaceStarted();
		
		bShowDebug = GameMode->IsDebugTextEnabled();
	}
}

void AKartPawn::SetCanMove(bool bInCanMove)
{
	bCanMove = bInCanMove;
}

FVector AKartPawn::GetMoveDirection() const
{
	return GetActorForwardVector() * FMath::Sign(CurrentSpeed);
}

void AKartPawn::EnterLapZone(int32 ZoneIndex)
{
	const AKartGameMode* GameMode = GetWorld()->GetAuthGameMode<AKartGameMode>();
	
	if (GameMode == nullptr || !GameMode->IsRaceStarted() || bHasFinishedRace)
	{
		return;
	}
	
	if (ZoneIndex == NextCheckpoint)
	{
		++NextCheckpoint;
	}
	else if (ZoneIndex == 0 && NextCheckpoint > GameMode->GetChackpointCount())
	{
		CompleteLap();
	}
}

void AKartPawn::CompleteLap()
{
	AKartGameMode* GameMode = GetWorld()->GetAuthGameMode<AKartGameMode>();
	if (GameMode == nullptr || bHasFinishedRace)
	{
		return;
	}
	
	NextCheckpoint = 1;
	
	if (CurrentLap >= GameMode->GetLapCount())
	{
		bHasFinishedRace = true;
		FinishTime = GameMode->GetRaceTime();
		GameMode->NotifyKartFinished(this);
		return;
	}
	
	++CurrentLap;
}

void AKartPawn::ResetRaceProgress()
{
	CurrentLap = 1;
	NextCheckpoint = 1;
	bHasFinishedRace = false;
	FinishTime = 0.f;
}

void AKartPawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(KartMappingContext, 0);
		}
	}
}

void AKartPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput == nullptr)
	{
		return;
	}
	
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AKartPawn::OnThrottle);
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AKartPawn::OnThrottleReleased);
	
	EnhancedInput->BindAction(SteerAction, ETriggerEvent::Triggered, this, &AKartPawn::OnSteer);
	EnhancedInput->BindAction(SteerAction, ETriggerEvent::Completed, this, &AKartPawn::OnSteerReleased);
	
	EnhancedInput->BindAction(DriftAction, ETriggerEvent::Started, this, &AKartPawn::OnDriftPressed);
	EnhancedInput->BindAction(DriftAction, ETriggerEvent::Completed, this, &AKartPawn::OnDriftReleased);
	
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AKartPawn::OnJumpPressed);
}

// Called every frame
void AKartPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanMove)
	{
		ThrottleInput = 0.f;
		SteerInput = 0.f;
	}
	
	UpdateGround(DeltaTime);
	UpdateBoost(DeltaTime);
	UpdateSpeed(DeltaTime);
	UpdateSteering(DeltaTime);
	UpdateDrift(DeltaTime);
	MoveKart(DeltaTime);
	AlignToGround(DeltaTime);
	UpdateVisuals(DeltaTime);
	UpdateCameraFOV(DeltaTime);
	
	if (bShowDebug)
	{
		ShowDebugInfo();
	}
}

void AKartPawn::OnThrottle(const FInputActionValue& Value)
{
	ThrottleInput = Value.Get<float>();
}
void AKartPawn::OnThrottleReleased(const FInputActionValue& Value)
{
	ThrottleInput = 0.f;
}
void AKartPawn::OnSteer(const FInputActionValue& Value)
{
	SteerInput = Value.Get<float>();
}
void AKartPawn::OnSteerReleased(const FInputActionValue& Value)
{
	SteerInput = 0.f;
}

void AKartPawn::OnDriftPressed(const FInputActionValue& Value)
{
	if (bCanMove)
	{
		StartDrift();
	}
}
void AKartPawn::OnDriftReleased(const FInputActionValue& Value)
{
	EndDrift();
}

void AKartPawn::OnJumpPressed(const FInputActionValue& Value)
{
	if (bCanMove)
	{
		Jump();
	}
}

void AKartPawn::UpdateGround(float DeltaTime)
{
	FVector GroundPoint;
	FVector WheelsNormal;
	const int32 WheelsOnFloor = TraceGroundUnderWheels(GroundPoint, WheelsNormal);
	const bool bHitFloor = WheelsOnFloor > 0;
	
	const float HeightAboveGround = FVector::DotProduct(GetActorLocation() - GroundPoint, GetActorUpVector());
	
	const float MaxGroundHeight = RideHeight + (bIsGrounded ? GroundStickDistance : GroundSnapTolerance);
	bIsGrounded = bHitFloor && HeightAboveGround <= MaxGroundHeight && VerticalSpeed <= 0.f;
	
	GroundNormal = bHitFloor ? WheelsNormal : FVector::UpVector;
	
	if (bIsGrounded)
	{
		VerticalSpeed = 0.f;
		
		const float  HeightError = HeightAboveGround - RideHeight;
		AddActorWorldOffset(-GetActorUpVector() * HeightError, true);
	}
	else
	{
		VerticalSpeed -= GravityStrength * DeltaTime;
	}
}

void AKartPawn::UpdateSpeed(float DeltaTime)
{
	if (BoostTimeRemaining > 0.f)
	{
		CurrentSpeed = BoostSpeed;
		return;
	}
	
	if (!bIsGrounded)
	{
		return;
	}
	
	float TargetSpeed = 0.f;
	float Rate = CoastDeceleration;
	
	if (ThrottleInput > 0.f)
	{
		TargetSpeed = GetCurrentMaxSpeed() * ThrottleInput;
		Rate = CurrentSpeed < 0.f ? BrakeDeceleration : Acceleration;
	}
	else if (ThrottleInput < 0.f)
	{
		TargetSpeed = MaxReverseSpeed * ThrottleInput;
		Rate = CurrentSpeed < 0.f ? BrakeDeceleration : Acceleration;
	}
	
	CurrentSpeed = FMath::FInterpConstantTo(CurrentSpeed, TargetSpeed, DeltaTime, Rate);
}

void AKartPawn::UpdateSteering(float DeltaTime)
{
	if (!bIsGrounded || FMath::Abs(CurrentSpeed) < MinSpeedToTurn)
	{
		return;
	}
	
	float Steer = SteerInput;
	float Rate = TurnRate;
	
	if (bIsDrifting)
	{
		const float SteerAlongDrift = SteerInput * DriftDirection;
		const float DriftSteer = DriftDirection * FMath::GetMappedRangeValueClamped(FVector2D(-1.f, 1.f), FVector2D(0.5f, 1.f), SteerAlongDrift);
		
		const float Blend = FMath::SmoothStep(0.f, 1.f, DriftBlend);
		Steer = FMath::Lerp(SteerInput, DriftSteer, Blend);
		Rate -= FMath::Lerp(1.f, DriftTurnMultiplier, Blend);
	}
	
	const float MoveDirection = CurrentSpeed >= 0.f ? 1.f : -1.f;
	const float YawDelta = Steer * Rate * MoveDirection * DeltaTime;
	AddActorLocalRotation(FRotator(0.f, YawDelta, 0.f));
}

void AKartPawn::UpdateDrift(float DeltaTime)
{
	if (!bIsDrifting)
	{
		return;
	}
	
	if (CurrentSpeed < DriftMinSpeed * 0.5f)
	{
		bIsDrifting = false;
		DriftDirection = 0;
		DriftTurnMultiplier = 0.f;
		return;
	}
	
	DriftBlend = FMath::Min(1.f, DriftBlend + DeltaTime / DriftEnterTime);
	
	if (bIsGrounded)
	{
		DriftChargeTime += DeltaTime;
	}
}

void AKartPawn::UpdateBoost(float DeltaTime)
{
	BoostTimeRemaining = FMath::Max(0.f, BoostTimeRemaining - DeltaTime);
}

void AKartPawn::MoveKart(float DeltaTime)
{
	FVector Forward = GetActorForwardVector();
	if (bIsGrounded)
	{
		Forward = FVector::VectorPlaneProject(Forward, GroundNormal).GetSafeNormal();
	}
	
	const FVector Delta = Forward * CurrentSpeed * DeltaTime + FVector::UpVector * VerticalSpeed * DeltaTime;
	
	FHitResult Hit;
	AddActorWorldOffset(Delta, true, &Hit);
	
	if (!Hit.bBlockingHit)
	{
		return;
	}
	
	const bool bHitWall = Hit.Normal.Z < WalkableFloorZ && !Hit.bStartPenetrating;
	if (bHitWall)
	{
		const FVector MoveDirection = Forward * FMath::Sign(CurrentSpeed);
		const float ImpactStrength = FMath::Clamp(-FVector::DotProduct(MoveDirection, Hit.Normal), 0.f, 1.f);
		
		CurrentSpeed *= FMath::Lerp(1.f, WallSpeedFactor, ImpactStrength);
	}
	else if (bIsGrounded && FVector::DotProduct(Hit.Normal, GroundNormal) >= WalkableFloorZ)
	{
		++SphereFloorHitCount;
		AddActorWorldOffset(Delta * (1.f - Hit.Time), false);
		return;
	}
	else if (VerticalSpeed < 0.f)
	{
		VerticalSpeed = 0.f;
	}
	
	const FVector RemainingDelta = FVector::VectorPlaneProject(Delta * (1.f - Hit.Time), Hit.Normal);
	AddActorWorldOffset(RemainingDelta, true);
}

void AKartPawn::AlignToGround(float DeltaTime)
{
	const FQuat TargetRotation = FRotationMatrix::MakeFromZX(GroundNormal, GetActorForwardVector()).ToQuat();
	
	const float Alpha = FMath::Clamp(GroundAlignSpeed * DeltaTime, 0.f, 1.f);
	const FQuat NewRotation = FQuat::Slerp(GetActorQuat(), TargetRotation, Alpha);
	
	SetActorRotation(NewRotation);
}

void AKartPawn::UpdateVisuals(float DeltaTime)
{
	WheelSpinAngle += FMath::RadiansToDegrees(CurrentSpeed * DeltaTime / WheelRadius);
	WheelSpinAngle = FMath::Fmod(WheelSpinAngle, 360.f);
	
	const float WheelSteerAngle = SteerInput * MaxWheelSteerAngle;
	SetWheelRotation(WheelFrontLeft, WheelFrontLeftBaseRotation, WheelSteerAngle);
	SetWheelRotation(WheelFrontRight, WheelFrontRightBaseRotation, WheelSteerAngle);
	SetWheelRotation(WheelRearLeft, WheelRearLeftBaseRotation, 0.f);
	SetWheelRotation(WheelRearRight, WheelRearRightBaseRotation, 0.f);
	
	const float TargetSlideYaw = bIsDrifting ? DriftDirection * DriftSlideAngle * FMath::SmoothStep(0.f, 1.f, DriftBlend) : 0.f;
	const float CurrentSlideYaw = BodyMesh->GetRelativeRotation().Yaw - BodyBaseRotation.Yaw;
	const float NewSlideYaw = FMath::FInterpTo(CurrentSlideYaw, TargetSlideYaw, DeltaTime, DriftSlideInterSpeed);
	
	FRotator BodyRotation = BodyBaseRotation;
	BodyRotation.Yaw += NewSlideYaw;
	BodyMesh->SetRelativeRotation(BodyRotation);
	
	CameraSlideYaw = FMath::FInterpTo(CameraSlideYaw, NewSlideYaw * CameraDriftFollow, DeltaTime, CameraDriftInterSpeed);
	
	const float KartPitch = GetActorRotation().Pitch;
	const float TargetSlopePitch = FMath::Abs(KartPitch) > CameraSlopeDeadZone ? KartPitch - FMath::Sign(KartPitch) * CameraSlopeDeadZone : 0.f;
	CameraSlopePitch = FMath::FInterpTo(CameraSlopePitch, TargetSlopePitch, DeltaTime, CameraSlopeFollowSpeed);
	
	FRotator ArmRotation = SpringArmBaseRotation;
	ArmRotation.Yaw += CameraSlideYaw;
	ArmRotation.Pitch += CameraSlopePitch;
	SpringArm-> SetRelativeRotation(ArmRotation);
}

void AKartPawn::UpdateCameraFOV(float DeltaTime)
{
	float TargetFOV = BaseFOV;
	
	if (BoostTimeRemaining > 0.f)
	{
		TargetFOV = BoostFOV;
	}
	else
	{
		const float SpeedRatio = FMath::Clamp(CurrentSpeed / MaxForwardSpeed, 0.f, 1.f);
		TargetFOV = FMath::Lerp(BaseFOV, MaxSpeedFOV, SpeedRatio);
	}
	
	const float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, FOVInterpSpeed);
	Camera->SetFieldOfView(NewFOV);
}

int32 AKartPawn::TraceGroundUnderWheels(FVector& OutGroundPoint, FVector& OutGroundNormal) const
{
	const FVector Up = GetActorUpVector();
	const FVector Forward = GetActorForwardVector() * GroundProbeHalfLength;
	const FVector Right = GetActorRightVector() * GroundProbeHalfWidth;
	
	const FVector WheelOffsets[4] = { Forward - Right, Forward + Right, -Forward - Right, -Forward + Right };
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	FVector Points[4];
	FVector PointSum = FVector::ZeroVector;
	FVector NormalSum = FVector::ZeroVector;
	int32 WheelsOnFloor = 0;
	
	for (int32 i = 0; i < 4; i++)
	{
		const FVector Start = GetActorLocation() + WheelOffsets[i] + Up * RideHeight;
		const FVector End = Start - Up * (RideHeight * 2.f + GroundTraceDistance);
		
		FHitResult Hit;
		const bool bHitFloor = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams) && FVector::DotProduct(Hit.ImpactNormal, Up) >= WalkableFloorZ;
		
		if (bHitFloor)
		{
			Points[i] = Hit.ImpactPoint;
			PointSum += Hit.ImpactPoint;
			NormalSum += Hit.ImpactNormal;
			++WheelsOnFloor;
		}
	}
		
	if (WheelsOnFloor == 0)
	{
		OutGroundPoint = GetActorLocation();
		OutGroundNormal = FVector::UpVector;
		return 0;
	}
		
	OutGroundPoint = PointSum / WheelsOnFloor;
		
	OutGroundNormal = NormalSum.GetSafeNormal();
		
	if (WheelsOnFloor == 4)
	{
		const FVector DiagonalA = Points[0] - Points[3];
		const FVector DiagonalB = Points[1] - Points[2];
		FVector PlaneNormal = FVector::CrossProduct(DiagonalA, DiagonalB).GetSafeNormal();
		
		if (FVector::DotProduct(PlaneNormal, Up) < 0.f)
		{
			PlaneNormal = -PlaneNormal;
		}
		
		if (!PlaneNormal.IsNearlyZero())
		{
			OutGroundNormal = PlaneNormal;
		}
	}
		
	return WheelsOnFloor;
}

void AKartPawn::SetWheelRotation(UStaticMeshComponent* Wheel, const FRotator& BaseRotation, float SteerAngle) const
{
	const FQuat SpinAndSteer = FQuat(FRotator(0.f, SteerAngle, WheelSpinAngle));
	Wheel->SetRelativeRotation(SpinAndSteer * FQuat(BaseRotation));
}

void AKartPawn::Jump()
{
	if (!bIsGrounded)
	{
		return;
	}
	
	VerticalSpeed = JumpVelocity;
	bIsGrounded = false;
}

void AKartPawn::StartDrift()
{
	if (CurrentSpeed < DriftMinSpeed || FMath::IsNearlyZero(SteerInput))
	{
		return;
	}
	
	bIsDrifting = true;
	DriftDirection = SteerInput > 0.f ? 1 : -1;
	DriftChargeTime = 0.f;
	DriftBlend = 0.f;
}

void AKartPawn::EndDrift()
{
	if (!bIsDrifting)
	{
		return;
	}
	
	if (DriftChargeTime >= MegaTurboChargeTime)
	{
		StartBoost(MegaTurboDuration);
	}
	else if (DriftChargeTime >= SuperTurboChargeTime)
	{
		StartBoost(SuperTurboDuration);
	}
	else if (DriftChargeTime >= MiniTurboChargeTime)
	{
		StartBoost(MiniTurboDuration);
	}
	
	bIsDrifting = false;
	DriftDirection = 0.f;
	DriftChargeTime = 0.f;
}

void AKartPawn::StartBoost(float Duration)
{
	BoostTimeRemaining = Duration;
	CurrentSpeed = BoostSpeed;
}

float AKartPawn::GetCurrentMaxSpeed() const
{
	return BoostTimeRemaining > 0.f ? BoostSpeed : MaxForwardSpeed;
}

void AKartPawn::ShowDebugInfo() const
{
	if (GEngine == nullptr)
	{
		return;
	}
	
	FColor DriftColor = FColor::White;
	if (DriftChargeTime >= MegaTurboChargeTime)
	{
		DriftColor = FColor::Magenta;
	}
	else if (DriftChargeTime >= SuperTurboChargeTime)
	{
		DriftColor = FColor::Orange;
	}
	else if (DriftChargeTime >= MiniTurboChargeTime)
	{
		DriftColor = FColor::Cyan;
	}
	
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, FString::Printf(TEXT("Speed: %.0f Grounded: %s"), CurrentSpeed, bIsGrounded ? TEXT("yes") : TEXT("no")));
	GEngine->AddOnScreenDebugMessage(2, 0.f, DriftColor, FString::Printf(TEXT("Drift: %s Grounded: %.2f"), bIsDrifting ? TEXT("yes") : TEXT("no"), DriftChargeTime));
	GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Yellow, FString::Printf(TEXT("Boost: %.2f"), BoostTimeRemaining));
}
