#pragma once

#include "CoreMinimal.h"
#include "Items/KartItemData.h"
#include "KartItemData_Boost.generated.h"

UCLASS()
class MARIOKART_API UKartItemData_Boost : public UKartItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Boost", meta = (ClampMin = 0.1f))
	float BoostDuration = 1.5f;
	
	virtual bool Activate(AKartPawn* Kart) const override;
};