#pragma once

#include "CoreMinimal.h"
#include "Kart/KartData.h"
#include "KartData_Wheels.generated.h"

class UStaticMesh;

UCLASS(BlueprintType)
class MARIOKART_API UKartData_Wheels : public UKartData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart")
	TObjectPtr<UStaticMesh> SecondMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart")
	float Raduis;
	
	UFUNCTION(BlueprintCallable)
	void SetWheels(AKartPawn* Kart) const;
	UFUNCTION(BlueprintCallable)
	void SetWheelsVisual(AVisualKart* Kart) const;
	
	void SetStat(AKartPawn* Kart) const override;
};