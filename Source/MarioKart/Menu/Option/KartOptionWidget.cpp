#include "Menu/Option/KartOptionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "InputCoreTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Menu/Option/KartOptionSelector.h"
#include "Settings/KartGameUserSettings.h"

void UKartOptionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	GraphicsTabButton->OnClicked.AddDynamic(this, &UKartOptionWidget::HandleGraphicsTabClicked);
	
	SetupGraphicsChoices();
	ResolutionSelector->OnSelectionChanged.AddDynamic(this, &UKartOptionWidget::HandleResolutionChanged);
	WindowModeSelector->OnSelectionChanged.AddDynamic(this, &UKartOptionWidget::HandleWindowModeChanged);
	QualitySelector->OnSelectionChanged.AddDynamic(this, &UKartOptionWidget::HandleQualityChanged);
	VSyncSelector->OnSelectionChanged.AddDynamic(this, &UKartOptionWidget::HandleVSyncChanged);
	FrameRateSelector->OnSelectionChanged.AddDynamic(this, &UKartOptionWidget::HandleFrameRateChanged);
	
	ApplyButton->OnClicked.AddDynamic(this, &UKartOptionWidget::HandleApplyClicked);
	DefaultButton->OnClicked.AddDynamic(this, &UKartOptionWidget::HandleDefaultClicked);
	BackButton->OnClicked.AddDynamic(this, &UKartOptionWidget::HandleBackClicked);
	KeepButton->OnClicked.AddDynamic(this, &UKartOptionWidget::HandleKeepClicked);
	RevertButton->OnClicked.AddDynamic(this, &UKartOptionWidget::HandleRevertClicked);
	
	ShowConfirmPanel(false);
	ShowTab(0);
	LoadFromSettings();
}

void UKartOptionWidget::FocusDefaultButton()
{
	GraphicsTabButton->SetUserFocus(GetOwningPlayer());
}

void UKartOptionWidget::HandleGraphicsTabClicked()
{
	ShowTab(0);
}

void UKartOptionWidget::ShowTab(int32 TabIndex)
{
	TabSwitcher->SetActiveWidgetIndex(TabIndex);
}

FReply UKartOptionWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	
	if (Key == EKeys::Escape || Key == EKeys::Gamepad_FaceButton_Right)
	{
		if (ConfirmTimeLeft > 0.f)
		{
			HandleRevertClicked();
		}
		else
		{
			HandleBackClicked();
		}
		return FReply::Handled();
	}
	
	if (Key == EKeys::Gamepad_LeftShoulder || Key == EKeys::Gamepad_RightShoulder)
	{
		const int32 TabCount = TabSwitcher->GetNumWidgets();
		const int32 Direction = Key == EKeys::Gamepad_LeftShoulder ? -1 : 1;
		ShowTab((TabSwitcher->GetActiveWidgetIndex() + Direction + TabCount) % TabCount);
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UKartOptionWidget::SetupGraphicsChoices()
{
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	
	if (const UKartGameUserSettings* Settings = UKartGameUserSettings::Get())
	{
		Resolutions.AddUnique(Settings->GetScreenResolution());
	}
	
	TArray<FText> ResolutionChoices;
	for (const FIntPoint& Resolution : Resolutions)
	{
		ResolutionChoices.Add(FText::FromString(FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y)));
	}
	ResolutionSelector->SetChoices(ResolutionChoices);
	
	WindowModeSelector->SetChoices({FText::FromString(TEXT("Plein écran")), FText::FromString(TEXT("Plein écran fenêtré")), FText::FromString(TEXT("Fenêtré"))});
	QualitySelector->SetChoices({FText::FromString(TEXT("Basse")), FText::FromString(TEXT("Moyenne")), FText::FromString(TEXT("Haute")), FText::FromString(TEXT("Épique"))});
	WindowModeSelector->SetChoices({FText::FromString(TEXT("Non")), FText::FromString(TEXT("Oui"))});
	
	TArray<FText> FrameRateChoices;
	for (const float Limit : FrameRateLimits)
	{
		FrameRateChoices.Add(Limit > 0.f ? FText::AsNumber(FMath::RoundToInt(Limit)) : FText::FromString(TEXT("Illimité")));
	}
	FrameRateSelector->SetChoices(FrameRateChoices);
}

void UKartOptionWidget::LoadFromSettings()
{
	const UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	
	ResolutionSelector->SetSelectedIndex(Resolutions.Find(Settings->GetScreenResolution()));
	WindowModeSelector->SetSelectedIndex(static_cast<int32>(Settings->GetFullscreenMode()));
	const int32 Quality = Settings->GetOverallScalabilityLevel();
	QualitySelector->SetSelectedIndex(Quality >= 0 ? Quality : 3);
	VSyncSelector->SetSelectedIndex(Settings->IsVSyncEnabled() ? 1 : 0);
	FrameRateSelector->SetSelectedIndex(FrameRateLimits.Find(Settings->GetFrameRateLimit()));
}

void UKartOptionWidget::HandleResolutionChanged(int32 NewIndex)
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr || !Resolutions.IsValidIndex(NewIndex))
	{
		return;
	}
	Settings->SetScreenResolution(Resolutions[NewIndex]);
}
void UKartOptionWidget::HandleWindowModeChanged(int32 NewIndex)
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	Settings->SetFullscreenMode(static_cast<EWindowMode::Type>(NewIndex));
}
void UKartOptionWidget::HandleQualityChanged(int32 NewIndex)
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	Settings->SetOverallScalabilityLevel(NewIndex);
}
void UKartOptionWidget::HandleVSyncChanged(int32 NewIndex)
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	Settings->SetVSyncEnabled(NewIndex == 1);
}
void UKartOptionWidget::HandleFrameRateChanged(int32 NewIndex)
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	Settings->SetFrameRateLimit(FrameRateLimits[NewIndex]);
}

void UKartOptionWidget::HandleApplyClicked()
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	
	const bool bVideoModeChanged = Settings->IsScreenResolutionDirty() || Settings->IsFullscreenModeDirty();
	Settings->ApplySettings(false);
	if (bVideoModeChanged)
	{
		ShowConfirmPanel(true);
	}
}
void UKartOptionWidget::HandleDefaultClicked()
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	
	Settings->SetToDefaults();
	LoadFromSettings();
}
void UKartOptionWidget::HandleBackClicked()
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	Settings->LoadSettings(true);
	RemoveFromParent();
	OnClosed.Broadcast();
}

void UKartOptionWidget::ShowConfirmPanel(bool bShow)
{
	ConfirmPanel->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ConfirmTimeLeft = bShow ? ConfirmDuration : 0.f;
	
	if (bShow)
	{
		KeepButton->SetUserFocus(GetOwningPlayer());
	}
	else
	{
		FocusDefaultButton();
	}
}
void UKartOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (ConfirmTimeLeft <= 0.f)
	{
		return;
	}
	
	ConfirmTimeLeft -= InDeltaTime;
	ConfirmText->SetText(FText::FromString(FString::Printf(TEXT("Garder ces réglages ? Retour automatique dans %d s"), FMath::CeilToInt(ConfirmTimeLeft))));
	
	if (ConfirmTimeLeft <= 0.f)
	{
		HandleRevertClicked();
	}
}

void UKartOptionWidget::HandleKeepClicked()
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	Settings->ConfirmVideoMode();
	Settings->SaveSettings();
	ShowConfirmPanel(false);
}
void UKartOptionWidget::HandleRevertClicked()
{
	UKartGameUserSettings* Settings = UKartGameUserSettings::Get();
	if (Settings == nullptr)
	{
		return;
	}
	Settings->RevertVideoMode();
	Settings->ApplyResolutionSettings(false);
	Settings->SaveSettings();
	ShowConfirmPanel(false);
	LoadFromSettings();
}