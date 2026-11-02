// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kart_PlayerController.generated.h"

class UKartCountDownWidget;
class UKartRaceHUDWidget;

UCLASS()
class MARIOKART_API AKart_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Kart|UI")
	TSubclassOf<UKartCountDownWidget> CountdownWidgetClass;
	UPROPERTY(EditAnywhere, Category="Kart|UI")
	TSubclassOf<UKartRaceHUDWidget> RaceHUDWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UKartCountDownWidget> CountdownWidget;
	UPROPERTY()
	TObjectPtr<UKartRaceHUDWidget> RaceHUDWidget;
};
