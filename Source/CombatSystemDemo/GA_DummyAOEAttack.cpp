#include "GA_DummyAOEAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

UGA_DummyAOEAttack::UGA_DummyAOEAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_DummyAOEAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Warning, TEXT("DummyAOE: CommitAbility FAILED"));
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

    UE_LOG(LogTemp, Warning, TEXT("DummyAOE: Hits found = %d"), Hits.Num());

    UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
    if (SourceASC && DamageEffectClass)
    {
        for (auto& Hit : Hits)
        {
            if (!Hit.GetActor()) continue;
            UE_LOG(LogTemp, Warning, TEXT("DummyAOE: Hit Actor = %s"), *Hit.GetActor()->GetName());

            UAbilitySystemComponent* TargetASC = nullptr;
            if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Hit.GetActor()))
                TargetASC = ASI->GetAbilitySystemComponent();

            UE_LOG(LogTemp, Warning, TEXT("DummyAOE: TargetASC valid = %s"), TargetASC ? TEXT("YES") : TEXT("NO"));

            if (!TargetASC) continue;

            FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
            FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, Ctx);
            UE_LOG(LogTemp, Warning, TEXT("DummyAOE: Spec valid = %s"), Spec.IsValid() ? TEXT("YES") : TEXT("NO"));

            if (Spec.IsValid())
            {
                Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Damage.Critical"), 0.f);
                FActiveGameplayEffectHandle AppliedHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
                UE_LOG(LogTemp, Warning, TEXT("DummyAOE: Effect Applied = %s"), AppliedHandle.WasSuccessfullyApplied() ? TEXT("YES") : TEXT("NO"));
            }
        }
    }

    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DummyAOE: SourceASC or DamageEffectClass missing"));
    }

    PlayAOEMontage();

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}