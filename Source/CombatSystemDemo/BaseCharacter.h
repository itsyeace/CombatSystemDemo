#pragma once
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "StanceComponent.h"
#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UBaseAttributeSet;

UCLASS()
class COMBATSYSTEMDEMO_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()
public:
    ABaseCharacter();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UGameplayEffect> DefaultAttributesEffect;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stance")
    UStanceComponent* StanceComp;

protected:
    UPROPERTY() UAbilitySystemComponent* CachedASC;
    UPROPERTY() UBaseAttributeSet* CachedAttributeSet;
    void InitAbilityActorInfo();
    void GiveDefaultAbilities();
    void ApplyDefaultAttributes();
};
