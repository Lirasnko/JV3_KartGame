#include "Menu/KartChoice/KartChoiceWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Kart/KartData.h"
#include "GameFramework/PlayerController.h"
#include "Kart/KartPartListData.h"
#include "Kismet/GameplayStatics.h"

void UKartChoiceWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	PlayButton->OnClicked.AddDynamic(this, &UKartChoiceWidget::HandlePlayClicked);
	BackButton->OnClicked.AddDynamic(this, &UKartChoiceWidget::HandleBackClicked);
	
	if (BodyList != nullptr)
	{
		for (int i = 0; i < BodyList->Parts.Num(); i++)
		{
			TObjectPtr<UButton> Button = CreateDefaultSubobject<UButton>(TEXT("Button_0_" + i));
			TObjectPtr<UImage> Image = CreateDefaultSubobject<UImage>(TEXT("Image_0_" + i));
			Image->Brush.SetResourceObject(BodyList->Parts[i].KartData->Icon);
			Image->SetBrushSize(FVector2D(128.f, 128.f));
			Button->AddChild(Image);
			VerticalBox_Body->AddChildToVerticalBox(Button);
		}
	}
	if (WheelsList != nullptr)
	{
		for (int i = 0; i < WheelsList->Parts.Num(); i++)
		{
			TObjectPtr<UButton> Button = CreateDefaultSubobject<UButton>(TEXT("Button_1_" + i));
			TObjectPtr<UImage> Image = CreateDefaultSubobject<UImage>(TEXT("Image_1_" + i));
			Image->Brush.SetResourceObject(WheelsList->Parts[i].KartData->Icon);
			Image->SetBrushSize(FVector2D(128.f, 128.f));
			Button->AddChild(Image);
			VerticalBox_Wheels->AddChildToVerticalBox(Button);
		}
	}
	if (SpoilerList != nullptr)
	{
		for (int i = 0; i < SpoilerList->Parts.Num(); i++)
		{
			TObjectPtr<UButton> Button = CreateDefaultSubobject<UButton>(TEXT("Button_2_" + i));
			TObjectPtr<UImage> Image = CreateDefaultSubobject<UImage>(TEXT("Image_2_" + i));
			Image->Brush.SetResourceObject(SpoilerList->Parts[i].KartData->Icon);
			Image->SetBrushSize(FVector2D(128.f, 128.f));
			Button->AddChild(Image);
			VerticalBox_Spoiler->AddChildToVerticalBox(Button);
		}
	}
}

void UKartChoiceWidget::FocusDefaultButton()
{
	PlayButton->SetUserFocus(GetOwningPlayer());
}

void UKartChoiceWidget::HandlePlayClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, RaceLevel);
}

void UKartChoiceWidget::HandleBackClicked()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuLevel);
}