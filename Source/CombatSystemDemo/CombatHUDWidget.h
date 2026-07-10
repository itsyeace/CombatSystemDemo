#pragma once
#include "Blueprint/UserWidget.h"
#include "CombatHUDWidget.generated.h"

UENUM(BlueprintType)
enum class EAbilityStatus : uint8
{
    Active,
    Disabled,
    OnCooldown
};

UCLASS()
class COMBATSYSTEMDEMO_API UCombatHUDWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdatePlayerHealth(float Current, float Max);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateDummyHealth(float Current, float Max);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateAmmo(int32 Current, int32 Max);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateComboCount(int32 Count);

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void ShowCriticalHitText();

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateAbilityStatus(EAbilityStatus Status);
};
