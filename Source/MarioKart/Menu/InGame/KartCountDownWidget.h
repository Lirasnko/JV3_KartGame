// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartCountDownWidget.generated.h"

class UTextBlock;

UCLASS()
class MARIOKART_API UKartCountDownWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	//le 3, 2, 1, GO
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountdownText;
	//Texte de start
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Countdown")
	FText GoText = NSLOCTEXT("Kart", "Go", "GO!");
	//Time before kill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Countdown", meta = (ClampMin = "0"))
	float GoDisplayTime = 1.f;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Kart|Countdown")
	void OnCountdownNumberShown(int32 Number);
	
private:
	UFUNCTION()
	void HandleCountdownTick(int32 Number);
	
	void HideCountdown();
	
	FTimerHandle HideTimer;
};
