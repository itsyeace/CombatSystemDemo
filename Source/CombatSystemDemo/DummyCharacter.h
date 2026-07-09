#pragma once
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "DummyCharacter.generated.h"

class UAbilitySystemComponent;
class UBaseAttributeSet;

UCLASS()
class COMBATSYSTEMDEMO_API ADummyCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()
public:
    ADummyCharacter();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
    UBaseAttributeSet* GetAttributeSet() const { return AttributeSet; }

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UGameplayEffect> DefaultAttributesEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    TSubclassOf<class UGameplayAbility> AutoAttackAbility;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackInterval = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float RespawnRadius = 800.f;

protected:
    UPROPERTY(VisibleAnywhere) UAbilitySystemComponent* AbilitySystemComponent;
    UPROPERTY(VisibleAnywhere) UBaseAttributeSet* AttributeSet;

    virtual void BeginPlay() override;

    FTimerHandle AttackTimerHandle;
    void PerformAutoAttack();

    FDelegateHandle HealthChangedHandle;
    void OnHealthChanged(const FOnAttributeChangeData& Data);

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void PlayHitReact();

    void HandleDeath();
    void RespawnAtRandomLocation();

    FVector InitialAnchorLocation;
    bool bIsDead = false;
};
