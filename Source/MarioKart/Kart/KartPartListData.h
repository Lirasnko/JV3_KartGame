#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KartPartListData.generated.h"

class UKartData;

USTRUCT(BlueprintType)
struct FKartPartListEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UKartData> KartData;
};

UCLASS(BlueprintType)
class MARIOKART_API UKartPartListData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TArray<FKartPartListEntry> Parts;
};