// Fill out your copyright notice in the Description page of Project Settings.

#include "Ramp.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARamp::ARamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = DetectionBox;
	
	DetectionBox->SetBoxExtent(FVector(30.f, 160.f, 40.f));
	
	DetectionBox->SetCollisionProfileName(TEXT("Trigger"));
	
	DetectionBox->SetGenerateOverlapEvents(true);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(DetectionBox);
	
	Arrow->SetArrowColor(FLinearColor::Green);
	Arrow->SetArrowSize(3.f);
	Arrow->SetArrowLength(100.f);
}

// Called when the game starts or when spawned
void ARamp::BeginPlay()
{
	Super::BeginPlay();
	
	DetectionBox->SetHiddenInGame(!bShowInGame);
	Arrow->SetHiddenInGame(!bShowInGame);
	
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ARamp::HandleBeginOverlap);
}

void ARamp::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

bool ARamp::IsGoingRightWay(const FVector& MoveDirection) const
{
	
}
