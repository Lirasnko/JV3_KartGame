#pragma once

#include "CoreMinimal.h"
#include "Kart/KartData.h"
#include "KartData_Body.generated.h"

class UStaticMesh;

UCLASS(BlueprintType)
class MARIOKART_API UKartData_Body : public UKartData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart|Body")
	TObjectPtr<UStaticMesh> LightMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart|Body")
	FVector LightPosition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart|Body")
	FRotator LightRotator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart|Body")
	FVector SpotLightPosition;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart|Body")
	FVector WheelFrontPosition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart|Body")
	FVector WheelRearPosition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart|Body")
	FVector SpoilerPosition;
	
	UFUNCTION(BlueprintCallable)
	void SetBody(AKartPawn* Kart) const;
	UFUNCTION(BlueprintCallable)
	void SetBodyVisual(AVisualKart* Kart) const;
	
	void SetStat(AKartPawn* Kart) const override;
	
};