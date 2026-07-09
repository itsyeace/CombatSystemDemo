#include "GA_DummyAOEAttack.h"
#include "AbilitySystemComponent.h"

UGA_DummyAOEAttack::UGA_DummyAOEAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_DummyAOEAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar) { EndAbility(Handle, ActorInfo, ActivationInfo, true, false); return; }

    TArray<FHitResult> Hits;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Avatar);

    GetWorld()->SweepMultiByChannel(Hits, Avatar->GetActorLocation(), Avatar->GetActorLocation(), FQuat::Identity, ECC_Pawn, Sphere, Params);

    UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
    if (SourceASC && DamageEffectClass)
    {
        for (auto& Hit : Hits)
        {
            if (!Hit.GetActor()) continue;
            UAbilitySystemComponent* TargetASC = Hit.GetActor()->FindComponentByClass<UAbilitySystemComponent>();
            if (!TargetASC) continue;

            FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
            FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, Ctx);
            if (Spec.IsValid())
            {
                Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Damage.Critical"), 0.f);
                SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
            }
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}