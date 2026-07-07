#pragma once
#include "Abilities/GameplayAbility.h"
#include "GA_MeleeCombo.generated.h"

UCLASS()
class COMBATSYSTEMDEMO_API UGA_MeleeCombo : public UGameplayAbility
{
    GENERATED_BODY()
public:
    UGA_MeleeCombo();

    UPROPERTY(EditDefaultsOnly, Category = "Combo")
    int32 ComboIndex = 1;

    UPROPERTY(EditDefaultsOnly, Category = "Combo")
    TSubclassOf<class UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Combo")
    float TraceRange = 150.f;

    UPROPERTY(EditDefaultsOnly, Category = "Combo")
    float TraceRadius = 50.f;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UFUNCTION(BlueprintCallable, Category = "Combo")
    void PerformTrace();

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Combo")
    void PlayComboMontage();
};
