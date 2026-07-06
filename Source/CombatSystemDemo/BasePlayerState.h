#pragma once
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "BasePlayerState.generated.h"

class UAbilitySystemComponent;
class UBaseAttributeSet;

UCLASS()
class COMBATSYSTEMDEMO_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()
public:
    ABasePlayerState();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
    UBaseAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
    UPROPERTY(VisibleAnywhere)
    UAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere)
    UBaseAttributeSet* AttributeSet;
};
