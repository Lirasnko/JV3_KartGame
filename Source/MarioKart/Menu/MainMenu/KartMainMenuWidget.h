#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartMainMenuWidget.generated.h"

class UButton;
class UKartOptionWidget;

UCLASS()
class MARIOKART_API UKartMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void FocusDefaultButton();
	
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
	UPROPERTY(EditAnywhere, Category = "Kart|Menu")
	TSoftObjectPtr<UWorld> KartChoiceLevel;
	UPROPERTY(EditAnywhere, Category = "Kart|Menu")
	TSubclassOf<UKartOptionWidget> OptionWidgetClass;
	
private:
	UFUNCTION()
	void HandlePlayClicked();
	UFUNCTION()
	void HandleOptionClicked();
	UFUNCTION()
	void HandleQuitClicked();
	
	UFUNCTION()
	void HandleOptionClosed();
	UPROPERTY()
	TObjectPtr<UKartOptionWidget> OptionWidget;
};