#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartChoiceWidget.generated.h"

class UButton;
class UVerticalBox;
class UKartPartListData;
class AVisualKart;

UCLASS()
class MARIOKART_API UKartChoiceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void FocusDefaultButton();
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Body;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Wheels;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Spoiler;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Menu")
	TSoftObjectPtr<UWorld> RaceLevel;
	UPROPERTY(EditAnywhere, Category = "Kart|Menu")
	TSoftObjectPtr<UWorld> MainMenuLevel;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Parts")
	TObjectPtr<AVisualKart> Kart;
	UPROPERTY(EditAnywhere, Category = "Kart|Parts")
	TSoftObjectPtr<UKartPartListData> BodyList;
	UPROPERTY(EditAnywhere, Category = "Kart|Parts")
	TSoftObjectPtr<UKartPartListData> WheelsList;
	UPROPERTY(EditAnywhere, Category = "Kart|Parts")
	TSoftObjectPtr<UKartPartListData> SpoilerList;
	
private:
	UFUNCTION()
	void HandlePlayClicked();
	UFUNCTION()
	void HandleBackClicked();
};