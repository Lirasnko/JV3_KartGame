#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartOptionWidget.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;
class UKartOptionSelector;
class UKartOptionSlider;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptionsClosed);

UCLASS()
class MARIOKART_API UKartOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void FocusDefaultButton();
	
	UPROPERTY(BlueprintAssignable, Category = "Kart|Option")
	FOnOptionsClosed OnClosed;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UWidgetSwitcher> TabSwitcher;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> GraphicsTabButton;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> AudioTabButton;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> GameTabButton;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> CommandTabButton;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSelector> ResolutionSelector;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSelector> WindowModeSelector;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSelector> QualitySelector;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSelector> VSyncSelector;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSelector> FrameRateSelector;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSlider> MasterVolumeSlider;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSlider> MusicVolumeSlider;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSlider> SfxVolumeSlider;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSlider> FOVSlider;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UKartOptionSlider> LapCountSlider;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> ApplyButton ;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> DefaultButton ;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> BackButton ;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UWidget> ConfirmPanel;
	UPROPERTY(meta =(BindWidget))
		TObjectPtr<UTextBlock> ConfirmText;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> KeepButton;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> RevertButton;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Option", Meta = (ClampMin = "1"))
	float ConfirmDuration = 10.f;
	
private:
	UFUNCTION()
	void HandleGraphicsTabClicked();
	UFUNCTION()
	void HandleAudioTabClicked();
	UFUNCTION()
	void HandleGameTabClicked();
	UFUNCTION()
	void HandleCommandTabClicked();
	void ShowTab(int32 TabIndex);
	
	void SetupGraphicsChoices();
	void LoadFromSettings();
	
	UFUNCTION()
	void HandleResolutionChanged(int32 NewIndex);
	UFUNCTION()
	void HandleWindowModeChanged(int32 NewIndex);
	UFUNCTION()
	void HandleQualityChanged(int32 NewIndex);
	UFUNCTION()
	void HandleVSyncChanged(int32 NewIndex);
	UFUNCTION()
	void HandleFrameRateChanged(int32 NewIndex);
	
	UFUNCTION()
	void HandleMasterVolumeChanged(float NewValue);
	UFUNCTION()
	void HandleMusicVolumeChanged(float NewValue);
	UFUNCTION()
	void HandleSfxVolumeChanged(float NewValue);
	
	UFUNCTION()
	void HandleFOVChanged(float NewValue);
	UFUNCTION()
	void HandleLapCountChanged(float NewValue);
	
	UFUNCTION()
	void HandleApplyClicked();
	UFUNCTION()
	void HandleDefaultClicked();
	UFUNCTION()
	void HandleBackClicked();
	
	UFUNCTION()
	void HandleKeepClicked();
	UFUNCTION()
	void HandleRevertClicked();
	
	void ShowConfirmPanel(bool bShow);
	
	TArray<FIntPoint> Resolutions;
	TArray<float> FrameRateLimits = {30.f, 60.f, 120.f, 144.f, 0.f};
	float ConfirmTimeLeft = 0.f;
};