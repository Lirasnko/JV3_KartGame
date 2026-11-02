// Fill out your copyright notice in the Description page of Project Settings.

#include "KartCountDownWidget.h"

#include "Components/TextBlock.h"
#include "KartGameMode.h"
#include "TimerManager.h"

void UKartCountDownWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Collapsed);
	
	if (AKartGameMode* GameMode = GetWorld()->GetAuthGameMode<AKartGameMode>())
	{
		GameMode->OnCountdownTick.AddUniqueDynamic(this, &UKartCountDownWidget::HandleCountdownTick);
		if (!GameMode->IsRaceStarted() && GameMode->GetCountdownNumber() > 0)
		{
			HandleCountdownTick(GameMode->GetCountdownNumber());
		}
	}
}

void UKartCountDownWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HideTimer);
		if (AKartGameMode* GameMode = World->GetAuthGameMode<AKartGameMode>())
		{
			GameMode->OnCountdownTick.RemoveDynamic(this, &UKartCountDownWidget::HandleCountdownTick);
		}
	}
	
	Super::NativeDestruct();
}

void UKartCountDownWidget::HandleCountdownTick(int32 Number)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	GetWorld()->GetTimerManager().ClearTimer(HideTimer);
	
	if (Number > 0)
	{
		CountdownText->SetText(FText::AsNumber(Number));
	}
	else
	{
		CountdownText->SetText(GoText);
		GetWorld()->GetTimerManager().SetTimer(HideTimer, this, &UKartCountDownWidget::HideCountdown, FMath::Max(GoDisplayTime, 0.01f), false);
	}
	
	OnCountdownNumberShown(Number);
}

void UKartCountDownWidget::HideCountdown()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
