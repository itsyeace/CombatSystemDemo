#pragma once
#include "Abilities/GameplayAbility.h"
#include "GA_DummyAOEAttack.generated.h"

UCLASS()
class COMBATSYSTEMDEMO_API UGA_DummyAOEAttack : public UGameplayAbility
{
    GENERATED_BODY()
public:
    UGA_DummyAOEAttack();

    UPROPERTY(EditDefaultsOnly, Category = "Attack")
    TSubclassOf<class UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Attack")
    float Radius = 250.f;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
