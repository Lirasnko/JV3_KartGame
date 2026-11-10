// Fill out your copyright notice in the Description page of Project Settings.

#include "Kart_PlayerController.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "KartCountDownWidget.h"
#include "KartPawn.h"
#include "KartRaceHUDWidget.h"
#include "Items/KartItemWidget.h"

void AKart_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (KartMappingContext != nullptr)
		{
			Subsystem->AddMappingContext(KartMappingContext, 0);
		}
	}
	if (CountdownWidgetClass != nullptr)
	{
		CountdownWidget = CreateWidget<UKartCountDownWidget>(this, CountdownWidgetClass);
		if (CountdownWidget != nullptr)
		{
			CountdownWidget->AddToViewport();
		}
	}
	if (ItemWidgetClass != nullptr)
	{
		ItemWidget = CreateWidget<UKartItemWidget>(this, ItemWidgetClass);
		if (ItemWidget != nullptr)
		{
			ItemWidget->AddToViewport();
		}
	}
	if (RaceHUDWidgetClass != nullptr)
	{
		RaceHUDWidget = CreateWidget<UKartRaceHUDWidget>(this, RaceHUDWidgetClass);
		if (RaceHUDWidget != nullptr)
		{
			RaceHUDWidget->AddToViewport();
		}
	}
}
void AKart_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInput == nullptr)
	{
		return;
	}
	
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AKart_PlayerController::OnThrottle);
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AKart_PlayerController::OnThrottleReleased);
	
	EnhancedInput->BindAction(SteerAction, ETriggerEvent::Triggered, this, &AKart_PlayerController::OnSteer);
	EnhancedInput->BindAction(SteerAction, ETriggerEvent::Completed, this, &AKart_PlayerController::OnSteerReleased);
	
	EnhancedInput->BindAction(DriftAction, ETriggerEvent::Started, this, &AKart_PlayerController::OnDriftPressed);
	EnhancedInput->BindAction(DriftAction, ETriggerEvent::Completed, this, &AKart_PlayerController::OnDriftReleased);
	
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AKart_PlayerController::OnJumpPressed);
	
	EnhancedInput->BindAction(UseItemAction, ETriggerEvent::Started, this, &AKart_PlayerController::OnUseItemPressed);
	
	EnhancedInput->BindAction(TrickAction, ETriggerEvent::Started, this, &AKart_PlayerController::OnTrickPressed);
}

AKartPawn* AKart_PlayerController::GetKart() const
{
	return GetPawn<AKartPawn>();
}

void AKart_PlayerController::OnThrottle(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->SetThrottleInput(Value.Get<float>());
	}
}
void AKart_PlayerController::OnThrottleReleased(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->SetThrottleInput(0.f);
	}
}
void AKart_PlayerController::OnSteer(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->SetSteerInput(Value.Get<float>());
	}
}
void AKart_PlayerController::OnSteerReleased(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->SetSteerInput(0.f);
	}
}
void AKart_PlayerController::OnDriftPressed(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->StartDrift();
	}
}
void AKart_PlayerController::OnDriftReleased(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->EndDrift();
	}
}
void AKart_PlayerController::OnJumpPressed(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->Jump();
	}
}
void AKart_PlayerController::OnUseItemPressed(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->UseItem();
	}
}
void AKart_PlayerController::OnTrickPressed(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->Trick();
	}
}