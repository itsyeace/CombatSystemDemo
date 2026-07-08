#pragma once
#include "Abilities/GameplayAbility.h"
#include "GA_DashAttack.generated.h"

UCLASS()
class COMBATSYSTEMDEMO_API UGA_DashAttack : public UGameplayAbility
{
    GENERATED_BODY()
public:
    UGA_DashAttack();

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashDistance = 600.f;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashDuration = 0.25f;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    TSubclassOf<class UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float TraceRadius = 80.f;

    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
        FGameplayTagContainer* OptionalRelevantTags) const override;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
    FTimerHandle DashTimerHandle;
    FVector DashStartLocation;
    FVector DashTargetLocation;
    float DashElapsed = 0.f;
    FGameplayAbilitySpecHandle CachedHandle;
    const FGameplayAbilityActorInfo* CachedActorInfo;
    FGameplayAbilityActivationInfo CachedActivationInfo;

    void TickDash();
    void FinishDash();
    void DoDamageTrace();
};
