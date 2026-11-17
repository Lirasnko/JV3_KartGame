#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KartMenuPlayerController.generated.h"

class UKartMainMenuWidget;

UCLASS()
class MARIOKART_API AKartMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Kart|UI")
	TSubclassOf<UKartMainMenuWidget> MainMenuWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UKartMainMenuWidget> MainMenuWidget;
};