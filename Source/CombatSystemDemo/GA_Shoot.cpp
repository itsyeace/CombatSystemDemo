#include "GA_Shoot.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "CombatSystemDemoCharacter.h"

UGA_Shoot::UGA_Shoot()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    ACombatSystemDemoCharacter* Char = Cast<ACombatSystemDemoCharacter>(GetAvatarActorFromActorInfo());
    if (!Char)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    PlayShootMontage();

    // Decrement ammo
    Char->CurrentAmmo = FMath::Max(0, Char->CurrentAmmo - 1);
    UE_LOG(LogTemp, Warning, TEXT("Ammo: %d"), Char->CurrentAmmo);

    // Trace from camera
    UCameraComponent* Cam = Char->FindComponentByClass<UCameraComponent>();
    FVector Start = Cam ? Cam->GetComponentLocation() : Char->GetActorLocation();
    FVector Forward = Cam ? Cam->GetForwardVector() : Char->GetActorForwardVector();
    FVector End = Start + Forward * TraceRange;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Char);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params);

    if (bHit && Hit.GetActor())
    {
        UAbilitySystemComponent* TargetASC = Hit.GetActor()->FindComponentByClass<UAbilitySystemComponent>();
        UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

        if (TargetASC && SourceASC && DamageEffectClass)
        {
            FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
            Ctx.AddHitResult(Hit);
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