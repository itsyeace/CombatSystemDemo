#pragma once
#include "Abilities/GameplayAbility.h"
#include "GA_Shoot.generated.h"

UCLASS()
class COMBATSYSTEMDEMO_API UGA_Shoot : public UGameplayAbility
{
    GENERATED_BODY()
public:
    UGA_Shoot();

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    TSubclassOf<class UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    float TraceRange = 5000.f;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
