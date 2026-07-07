#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "BasePlayerState.h"
#include "BaseAttributeSet.h"

ABaseCharacter::ABaseCharacter() 
{
    StanceComp = CreateDefaultSubobject<UStanceComponent>("StanceComp");
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const { return CachedASC; }

void ABaseCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    InitAbilityActorInfo();
}

void ABaseCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    InitAbilityActorInfo();
}

void ABaseCharacter::InitAbilityActorInfo()
{
    if (ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
    {
        CachedASC = PS->GetAbilitySystemComponent();
        CachedAttributeSet = PS->GetAttributeSet();
        CachedASC->InitAbilityActorInfo(PS, this);
        GiveDefaultAbilities();
        ApplyDefaultAttributes();
    }
}

void ABaseCharacter::GiveDefaultAbilities()
{
    if (!CachedASC || !HasAuthority()) return;
    for (auto& AbilityClass : DefaultAbilities)
        CachedASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0));
}

void ABaseCharacter::ApplyDefaultAttributes()
{
    if (!CachedASC || !DefaultAttributesEffect) return;
    FGameplayEffectContextHandle Ctx = CachedASC->MakeEffectContext();
    FGameplayEffectSpecHandle Spec = CachedASC->MakeOutgoingSpec(DefaultAttributesEffect, 1, Ctx);
    if (Spec.IsValid())
    {
        CachedASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
        if (CachedAttributeSet)
        {
            UE_LOG(LogTemp, Warning, TEXT("Health: %f, MaxHealth: %f, Damage: %f"),
                CachedAttributeSet->GetHealth(), CachedAttributeSet->GetMaxHealth(), CachedAttributeSet->GetDamage());
        }
    }
}