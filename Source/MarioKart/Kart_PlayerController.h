// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kart_PlayerController.generated.h"

class UKartCountDownWidget;
class UKartRaceHUDWidget;
class UKartItemWidget;
class UInputMappingContext;
class UInputAction;
class AKartPawn;

struct FInputActionValue;

UCLASS()
class MARIOKART_API AKart_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditAnywhere, Category="Kart|UI")
	TSubclassOf<UKartCountDownWidget> CountdownWidgetClass;
	UPROPERTY(EditAnywhere, Category="Kart|UI")
	TSubclassOf<UKartRaceHUDWidget> RaceHUDWidgetClass;
	UPROPERTY(EditAnywhere, Category="Kart|UI")
	TSubclassOf<UKartItemWidget> ItemWidgetClass;
	
	/*----------Input----------*/
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
	//Bouton : Tricks
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> TrickAction;
	//Bouton : Items
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> UseItemAction;
	
private:
	void OnThrottle(const FInputActionValue& Value);
	void OnThrottleReleased(const FInputActionValue& Value);
	void OnSteer(const FInputActionValue& Value);
	void OnSteerReleased(const FInputActionValue& Value);
	void OnDriftPressed(const FInputActionValue& Value);
	void OnDriftReleased(const FInputActionValue& Value);
	void OnJumpPressed(const FInputActionValue& Value);
	void OnTrickPressed(const FInputActionValue& Value);
	void OnUseItemPressed(const FInputActionValue& Value);
	
	AKartPawn* GetKart() const;
	
	UPROPERTY()
	TObjectPtr<UKartCountDownWidget> CountdownWidget;
	UPROPERTY()
	TObjectPtr<UKartRaceHUDWidget> RaceHUDWidget;
	UPROPERTY()
	TObjectPtr<UKartItemWidget> ItemWidget;
};
