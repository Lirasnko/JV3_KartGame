#include "Menu/MainMenu/KartMainMenuWidget.h"

#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Menu/Option/KartOptionWidget.h"

void UKartMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	PlayButton->OnClicked.AddDynamic(this, &UKartMainMenuWidget::HandlePlayClicked);
	OptionButton->OnClicked.AddDynamic(this, &UKartMainMenuWidget::HandleOptionClicked);
	QuitButton->OnClicked.AddDynamic(this, &UKartMainMenuWidget::HandleQuitClicked);
}

void UKartMainMenuWidget::FocusDefaultButton()
{
	PlayButton->SetUserFocus(GetOwningPlayer());
}

void UKartMainMenuWidget::HandlePlayClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, KartChoiceLevel);
}
void UKartMainMenuWidget::HandleOptionClicked()
{
	if (OptionWidgetClass == nullptr)
	{
		return;
	}
	OptionWidget = CreateWidget<UKartOptionWidget>(GetOwningPlayer(), OptionWidgetClass);
	if (OptionWidget == nullptr)
	{
		return;
	}
	
	OptionWidget->OnClosed.AddDynamic(this, &UKartMainMenuWidget::HandleOptionClosed);
	OptionWidget->AddToViewport();
	OptionWidget->FocusDefaultButton();
	
	SetVisibility(ESlateVisibility::Collapsed);
}
void UKartMainMenuWidget::HandleQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
void UKartMainMenuWidget::HandleOptionClosed()
{
	OptionWidget = nullptr;
	SetVisibility(ESlateVisibility::Visible);
	FocusDefaultButton();
}
