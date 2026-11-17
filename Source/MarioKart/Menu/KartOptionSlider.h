#pragma once

#include "CoreMinimal.h"
#include "Menu/KartOptionRow.h"
#include "KartOptionSlider.generated.h"

class USlider

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionValueChanged, float, NewValue);