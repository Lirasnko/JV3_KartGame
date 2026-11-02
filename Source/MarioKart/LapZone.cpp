// Fill out your copyright notice in the Description page of Project Settings.


#include "LapZone.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "KartPawn.h"

// Sets default values
ALapZone::ALapZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	
	Box->SetBoxExtent(FVector(50.f, 500.f, 200.f));
	
	Box->SetCollisionProfileName(TEXT("Trigger"));
	
	Box->SetGenerateOverlapEvents(true);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	
	Arrow->SetArrowColor(FLinearColor::Green);
	Arrow->SetArrowSize(3.f);
	Arrow->SetArrowLength(100.f);
}

// Called when the game starts or when spawned
void ALapZone::BeginPlay()
{
	Super::BeginPlay();
	
	Box->SetHiddenInGame(!bShowInGame);
	Arrow->SetHiddenInGame(!bShowInGame);
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ALapZone::HandleBeginOverlap);
}

bool ALapZone::IsGoingRightWay(const FVector& MoveDirection) const
{
	return FVector::DotProduct(MoveDirection, Arrow->GetForwardVector()) > 0.f;
}

void ALapZone::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKartPawn* Kart = Cast<AKartPawn>(OtherActor);
	if (Kart == nullptr)
	{
		return;
	}
	
	if (!IsGoingRightWay(Kart->GetMoveDirection()))
	{
		return;
	}
	
	Kart->EnterLapZone(ZoneIndex);
}
