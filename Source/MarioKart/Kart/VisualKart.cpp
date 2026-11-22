
#include "VisualKart.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

AVisualKart::AVisualKart()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = BodyMesh;
	
	WheelFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFrontLeft"));
	WheelFrontLeft->SetupAttachment(BodyMesh);
	WheelFrontLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WheelFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFrontRight"));
	WheelFrontRight->SetupAttachment(BodyMesh);
	WheelFrontRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WheelRearLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelRearLeft"));
	WheelRearLeft->SetupAttachment(BodyMesh);
	WheelRearLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WheelRearRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelRearRight"));
	WheelRearRight->SetupAttachment(BodyMesh);
	WheelRearRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SpoilerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpoilerMesh"));
	SpoilerMesh->SetupAttachment(BodyMesh);
	SpoilerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	LightMesh->SetupAttachment(BodyMesh);
	LightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AVisualKart::SetBodyHeight(float BodyHeight)
{
	BodyMesh->SetRelativeLocation(FVector(0, 0,BodyHeight));
}

void AVisualKart::SetKartBody(UStaticMesh* BodySMesh, UStaticMesh* LightSMesh, FVector LightPosition)
{
	BodyMesh->SetStaticMesh(BodySMesh);
	if (LightSMesh == nullptr)
	{
		return;
	}
	LightMesh->SetStaticMesh(LightSMesh);
	LightMesh->SetRelativeLocation(LightPosition);
}
void AVisualKart::SetKartPartPosition(FVector WheelFrontPosition, FVector WheelRearPosition, FVector SpoilerPosition)
{
	WheelFrontLeft->SetRelativeLocation(FVector(-WheelFrontPosition.X, WheelFrontPosition.Y, WheelFrontPosition.Z));
	WheelFrontRight->SetRelativeLocation(WheelFrontPosition);
	WheelRearLeft->SetRelativeLocation(FVector(-WheelRearPosition.X, WheelRearPosition.Y, WheelRearPosition.Z));
	WheelRearRight->SetRelativeLocation(WheelRearPosition);
	SpoilerMesh->SetRelativeLocation(SpoilerPosition);
}
void AVisualKart::SetKartWheels(UStaticMesh* WheelLeftSMesh, UStaticMesh* WheelRightMesh)
{
	WheelFrontLeft->SetStaticMesh(WheelLeftSMesh);
	WheelFrontRight->SetStaticMesh(WheelRightMesh);
	WheelRearLeft->SetStaticMesh(WheelLeftSMesh);
	WheelRearRight->SetStaticMesh(WheelRightMesh);
}
void AVisualKart::SetKartSpoiler(UStaticMesh* SpoilerSMesh)
{
	SpoilerMesh->SetStaticMesh(SpoilerSMesh);
}