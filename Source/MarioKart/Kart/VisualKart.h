#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualKart.generated.h"

class UStaticMeshComponent;

UCLASS()
class MARIOKART_API AVisualKart : public AActor
{
	GENERATED_BODY()
	
public:
	AVisualKart();	
	
	UFUNCTION(BlueprintCallable, Category = "Kart|Stat")
	void SetBodyHeight(float BodyHeight);
	UFUNCTION(BlueprintCallable, Category = "Kart|Visual")
	void SetKartBody(UStaticMesh* BodySMesh, UStaticMesh* LightSMesh, FVector LightPosition);
	UFUNCTION(BlueprintCallable, Category = "Kart|Visual")
	void SetKartPartPosition(FVector WheelFrontPosition, FVector WheelRearPosition, FVector SpoilerPosition);
	UFUNCTION(BlueprintCallable, Category = "Kart|Visual")
	void SetKartWheels(UStaticMesh* WheelLeftSMesh, UStaticMesh* WheelRightMesh);
	UFUNCTION(BlueprintCallable, Category = "Kart|Visual")
	void SetKartSpoiler(UStaticMesh* SpoilerSMesh);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> WheelFrontLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> WheelFrontRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> WheelRearLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> WheelRearRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> SpoilerMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Components")
	TObjectPtr<UStaticMeshComponent> LightMesh;
};