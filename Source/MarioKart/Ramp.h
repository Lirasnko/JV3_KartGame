// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ramp.generated.h"

class UBoxComponent;
class UArrowComponent;
class UStaticMeshComponent;
struct FHitResult;

UCLASS()
class MARIOKART_API ARamp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARamp();
	
	UFUNCTION(BlueprintPure, Category = "Ramp")
	bool IsGoingRightWay(const FVector& MoveDirection) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ramp|Components")
	TObjectPtr<UBoxComponent> DetectionBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ramp|Components")
	TObjectPtr<UStaticMeshComponent> BoxMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ramp|Components")
	TObjectPtr<UArrowComponent> Arrow;
	
	UPROPERTY(EditAnywhere, Category = "Ramp|Debug")
	bool bShowInGame = true;
	
private:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};