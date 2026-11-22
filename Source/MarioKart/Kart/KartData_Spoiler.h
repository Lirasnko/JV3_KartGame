#pragma once

#include "CoreMinimal.h"
#include "Kart/KartData.h"
#include "KartData_Spoiler.generated.h"

class UStaticMesh;

UCLASS(BlueprintType)
class MARIOKART_API UKartData_Spoiler : public UKartData
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetSpoiler(AKartPawn* Kart) const;
	UFUNCTION(BlueprintCallable)
	void SetSpoilerVisual(AVisualKart* Kart) const;
	
	void SetStat(AKartPawn* Kart) const override;
};