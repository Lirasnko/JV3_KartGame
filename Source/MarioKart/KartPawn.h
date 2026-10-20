// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KartPawn.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS()
class MARIOKART_API AKartPawn : public APawn
{
	GENERATED_BODY()

public:
	AKartPawn();	
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "Kart")
	void SetCanMove(bool bInCanMove);
	
	UFUNCTION(BlueprintPure, Category = "Kart")
	bool CanMove() const { return bCanMove; }
	
	UFUNCTION(BlueprintPure, Category = "Kart")
	FVector GetMoveDirection() const;

	/*---------Race Lap----------*/
	void EnterLapZone(int32 ZoneIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Kart|Race")
	void CompleteLap();
	UFUNCTION(BlueprintCallable, Category = "Kart|Race")
	void ResetRaceProgress();
	UFUNCTION(BlueprintPure, Category = "Kart|Race")
	int32 GetCurrentLap() const { return CurrentLap; }
	UFUNCTION(BlueprintPure, Category = "Kart|Race")
	bool HasFinishedRace() const { return bHasFinishedRace; }
	UFUNCTION(BlueprintPure, Category = "Kart|Race")
	float GetFinishedTime() const { return FinishTime; }

protected:
	virtual void BeginPlay() override;
	
	virtual void NotifyControllerChanged() override;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<USphereComponent> CollisionSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> WheelFrontLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> WheelFrontRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> WheelRearLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> WheelRearRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UCameraComponent> Camera;
	
	/*----------Input------------*/
	
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputMappingContext> KartMappingContext;
	
	//Axe 1D : +1 Accélérer, -1 Freiner / Reculer
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> ThrottleAction;
	//Axe 1D : +1 Droite, -1 Gauche
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> SteerAction;
	//Bouton : Maintenir
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> DriftAction;
	//Bouton : Petit saut
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> JumpAction;
	
	/*----------Réglage Movement------------*/
	
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float MaxForwardSpeed = 2500.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float MaxReverseSpeed = 800.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float Acceleration = 1500.f;
	//Décélération quand on appuie dans le sens opposé
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float BrakeDeceleration = 3000.f;
	//Décélération quand on lâche les gaz
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float CoastDeceleration = 800.f;
	//Vitesse de rotation en degré/sec
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float TurnRate = 100.f;
	//En dessous de cette vitesse le kart ne tourne pas
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float MinSpeedToTurn = 50.f;
	//
	UPROPERTY(EditAnywhere, Category = "Kart|Movement", meta = (ClampMin = "0", ClampMax = "1"))
	float WallSpeedFactor = 0.3f;
	
	/*----------Réglage Gravité------------*/
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GravityStrength = 2500.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundTraceDistance = 80.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float RideHeight = 45.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundStickDistance = 30.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundAlignSpeed = 8.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundProbeHalfLength = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundProbeHalfWidth = 50.f;
	
	/*----------Réglage Saut------------*/
	
	UPROPERTY(EditAnywhere, Category = "Kart|Jump")
	float JumpVelocity = 450.f;
	
	/*----------Réglage Saut------------*/
	
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftMinSpeed = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftTurnMultiplier = 1.4f;
	//Angle visuel du châssis
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftSlideAngle = 25.f;
	//Time in sec for enter in drift
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftEnterTime = 0.4f;
	//Speed châssis to angle (petit + doux)
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftSlideInterSpeed = 6.f;
	
	//Time for Mini-Turbo
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float MiniTurboChargeTime = 1.f;
	//Time for Super-Turbo
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float SuperTurboChargeTime = 2.f;
	//Time for Mega-Turbo
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float MegaTurboChargeTime = 3.5f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float MiniTurboDuration = 0.6f;
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float SuperTurboDuration = 1.2f;
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float MegaTurboDuration = 2.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float BoostSpeed = 3500.f;
	
	/*----------Réglage Visuel------------*/
	
	UPROPERTY(EditAnywhere, Category = "Kart|Visual")
	float WheelRadius = 20.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Visual")
	float MaxWheelSteerAngle = 30.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float BaseFOV = 90.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float MaxSpeedFOV = 100.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float BoostFOV = 110.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Camera", meta = (ClampMin = "0", ClampMax = "1"))
	float CameraDriftFollow = 1.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float CameraDriftInterSpeed = 3.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Camera", meta = (ClampMin = "0", ClampMax = "90"))
	float CameraSlopeDeadZone = 10.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float CameraSlopeFollowSpeed = 3.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float FOVInterpSpeed = 4.f;
	UPROPERTY(EditAnywhere, Category = "Kart|Visual")
	bool bShowDebug = true;
	
	/*----------State------------*/
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float CurrentSpeed = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float VerticalSpeed = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float ThrottleInput = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float SteerInput = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	bool bIsGrounded = false;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	bool bIsDrifting = false;
	//*1 Gauche, +1 Droite
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	int32 DriftDirection = 0;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float DriftChargeTime = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float DriftBlend = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float BoostTimeRemaining = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float WheelSpinAngle = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	FVector GroundNormal = FVector::UpVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|State")
	bool bCanMove = true;
	
	/*------------Lap------------*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Race")
	int32 CurrentLap = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Race")
	int32 NextCheckpoint = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Race")
	bool bHasFinishedRace = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Race")
	float FinishTime = 0.f;
	
private:
	
	void OnThrottle(const FInputActionValue& Value);
	void OnThrottleReleased(const FInputActionValue& Value);
	void OnSteer(const FInputActionValue& Value);
	void OnSteerReleased(const FInputActionValue& Value);
	void OnDriftPressed(const FInputActionValue& Value);
	void OnDriftReleased(const FInputActionValue& Value);
	void OnJumpPressed(const FInputActionValue& Value);
	
	void UpdateGround(float DeltaTime);
	void UpdateSpeed(float DeltaTime);
	void UpdateSteering(float DeltaTime);
	void UpdateDrift(float DeltaTime);
	void UpdateBoost(float DeltaTime);
	void MoveKart(float DeltaTime);
	void AlignToGround(float DeltaTime);
	void UpdateVisuals(float DeltaTime);
	void UpdateCameraFOV(float DeltaTime);
	void ShowDebugInfo() const;
	
	int32 TraceGroundUnderWheels(FVector& OutGroundPoint, FVector& OutGroundNormal) const;
	
	void Jump();
	
	void StartDrift();
	void EndDrift();
	void StartBoost(float Duration);
	float GetCurrentMaxSpeed() const;
	
	void SetWheelRotation(UStaticMeshComponent* Wheel, const FRotator& BaseRotation, float SteerAngle) const;
	
	FRotator BodyBaseRotation;
	FRotator SpringArmBaseRotation;
	FRotator WheelFrontLeftBaseRotation;
	FRotator WheelFrontRightBaseRotation;
	FRotator WheelRearLeftBaseRotation;
	FRotator WheelRearRightBaseRotation;
	
	float CameraSlideYaw = 0.f;
	float CameraSlopePitch = 0.f;
	int32 SphereFloorHitCount = 0;
	
};
