// Fill out your copyright notice in the Description page of Project Settings.

#include "Kart_PlayerController.h"

#include "KartCountDownWidget.h"
#include "KartRaceHUDWidget.h"

void AKart_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	if (CountdownWidgetClass != nullptr)
	{
		CountdownWidget = CreateWidget<UKartCountDownWidget>(this, CountdownWidgetClass);
		if (CountdownWidget != nullptr)
		{
			CountdownWidget->AddToViewport();
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