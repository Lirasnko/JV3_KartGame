#pragma once

#include "CoreMinimal.h"
#include "Items/KartItemData.h"
#include "KartItemData_SpawnActor.generated.h"

UCLASS()
class MARIOKART_API UKartItemData_SpawnActor : public UKartItemData
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Spawn")
    TSubclassOf<AActor> ActorClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Spawn")
    FVector SpawnOffset = FVector(-150.f, 0.f, 0.f);
    
    virtual bool Activate(AKartPawn* Kart) const override;
};