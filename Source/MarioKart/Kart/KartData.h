#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KartData.generated.h"

class UStaticMesh;
class AKartPawn;
class AVisualKart;

UENUM()
enum class EKartData_ID : uint8
{
	AE86InitialD UMETA(DisplayName = "InitialD"),
	Citroen2CV UMETA(DisplayName = "2CV"),
	DeLorean UMETA(DisplayName = "DeLorean"),
	FordMustang UMETA(DisplayName = "Ford Mustang"),
	LamborghiniCountach UMETA(DisplayName = "Countach"),
	NissanSkylineGTRR34 UMETA(DisplayName = "Skyline"),
	Porsche911 UMETA(DisplayName = "911"),
	VolkswagenCombiT1 UMETA(DisplayName = "Combi"),
};

UCLASS(Abstract, BlueprintType)
class MARIOKART_API UKartData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart")
	EKartData_ID ID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart")
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart")
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart", meta = (ClampMin = "0", ClampMax = "1"))
	float StatSpeed = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart", meta = (ClampMin = "0", ClampMax = "1"))
	float StatAcceleration = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart", meta = (ClampMin = "0", ClampMax = "1"))
	float StatHeight = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart", meta = (ClampMin = "0", ClampMax = "1"))
	float StatHandling = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kart", meta = (ClampMin = "0", ClampMax = "1"))
	float StatTraction = 1.f;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetStat(AKartPawn* Kart) const;
};