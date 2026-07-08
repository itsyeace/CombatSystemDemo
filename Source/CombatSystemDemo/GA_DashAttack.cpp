#include "GA_DashAttack.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "StanceComponent.h"
#include "CombatSystemDemoCharacter.h"

UGA_DashAttack::UGA_DashAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_DashAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
    FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
        return false;

    ACombatSystemDemoCharacter* Char = Cast<ACombatSystemDemoCharacter>(ActorInfo->AvatarActor.Get());
    if (!Char || !Char->StanceComp || !Char->StanceComp->IsMelee())
        return false;

    return true;
}

void UGA_DashAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!Char)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    CachedHandle = Handle;
    CachedActorInfo = ActorInfo;
    CachedActivationInfo = ActivationInfo;

    DashStartLocation = Char->GetActorLocation();
    DashTargetLocation = DashStartLocation + Char->GetActorForwardVector() * DashDistance;
    DashElapsed = 0.f;

    GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UGA_DashAttack::TickDash, 0.016f, true);
}

void UGA_DashAttack::TickDash()
{
    ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!Char) { FinishDash(); return; }

    DashElapsed += 0.016f;
    float Alpha = FMath::Clamp(DashElapsed / DashDuration, 0.f, 1.f);
    FVector NewLoc = FMath::Lerp(DashStartLocation, DashTargetLocation, Alpha);
    Char->SetActorLocation(NewLoc, true);

    if (Alpha >= 1.f)
    {
        FinishDash();
    }
}

void UGA_DashAttack::FinishDash()
{
    GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
    DoDamageTrace();
    EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
}

void UGA_DashAttack::DoDamageTrace()
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar) return;

    TArray<FHitResult> Hits;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Avatar);

    GetWorld()->SweepMultiByChannel(Hits, Avatar->GetActorLocation(), Avatar->GetActorLocation(), FQuat::Identity, ECC_Pawn, Sphere, Params);

    UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
    if (!SourceASC || !DamageEffectClass) return;

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