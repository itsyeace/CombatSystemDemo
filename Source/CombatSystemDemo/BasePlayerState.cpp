#include "BasePlayerState.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"

ABasePlayerState::ABasePlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}