#include "GA_MeleeCombo.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameplayEffectTypes.h"

UGA_MeleeCombo::UGA_MeleeCombo()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MeleeCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    PlayComboMontage();
}

void UGA_MeleeCombo::PerformTrace()
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar) return;

    FVector Start = Avatar->GetActorLocation();
    FVector Forward = Avatar->GetActorForwardVector();
    FVector End = Start + Forward * TraceRange;

    TArray<FHitResult> Hits;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Avatar);

    bool bHit = GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, Sphere, Params);

    if (bHit)
    {
        for (auto& Hit : Hits)
        {
            AActor* Target = Hit.GetActor();
            if (!Target) continue;

            UAbilitySystemComponent* TargetASC = Target->FindComponentByClass<UAbilitySystemComponent>();
            if (!TargetASC) continue;

            UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
            if (!SourceASC || !DamageEffectClass) continue;

            FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
            Ctx.AddHitResult(Hit);
            FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, Ctx);

            if (Spec.IsValid())
            {
                bool bIsCrit = (ComboIndex == 3);
                Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Damage.Critical"), bIsCrit ? 1.f : 0.f);
                SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
            }
        }
    }

    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}