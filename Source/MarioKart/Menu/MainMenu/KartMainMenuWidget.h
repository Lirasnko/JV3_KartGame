#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartMainMenuWidget.generated.h"

class UButton;

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
	TObjectPtr<UButton> QuitButton;
	UPROPERTY(EditAnywhere, Category = "Kart|Menu")
	TSoftObjectPtr<UWorld> KartChoiceLevel;
	
private:
	UFUNCTION()
	void HandlePlayClicked();
	UFUNCTION()
	void HandleQuitClicked();
};