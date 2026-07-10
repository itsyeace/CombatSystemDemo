#include "DummyCharacter.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CombatSystemDemoPlayerController.h"
#include "CombatHUDWidget.h"

ADummyCharacter::ADummyCharacter()
{
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

void ADummyCharacter::BeginPlay()
{
    Super::BeginPlay();
    InitialAnchorLocation = GetActorLocation();

    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    if (DefaultAttributesEffect)
    {
        FGameplayEffectContextHandle Ctx = AbilitySystemComponent->MakeEffectContext();
        FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributesEffect, 1, Ctx);
        if (Spec.IsValid()) AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
    }

    if (AutoAttackAbility)
    {
        AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AutoAttackAbility, 1, 0));
    }

    HealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
        .AddUObject(this, &ADummyCharacter::OnHealthChanged);

    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ADummyCharacter::PerformAutoAttack, AttackInterval, true);
}

void ADummyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
    PushHealthToHUD();

    if (bIsDead) return;

    if (Data.NewValue < Data.OldValue)
    {
        PlayHitReact(); // BP hooks anim + VFX + SFX
    }

    if (Data.NewValue <= 0.f)
    {
        HandleDeath();
    }
}

void ADummyCharacter::HandleDeath()
{
    bIsDead = true;
    GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
    SetActorEnableCollision(false);
    GetMesh()->SetVisibility(false);

    FTimerHandle RespawnDelay;
    GetWorld()->GetTimerManager().SetTimer(RespawnDelay, this, &ADummyCharacter::RespawnAtRandomLocation, 2.0f, false);
}

void ADummyCharacter::RespawnAtRandomLocation()
{
    ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
    FVector Anchor = PlayerChar ? PlayerChar->GetActorLocation() : InitialAnchorLocation;

    FVector RandomOffset = FVector(FMath::RandRange(-RespawnRadius, RespawnRadius), FMath::RandRange(-RespawnRadius, RespawnRadius), 0.f);
    SetActorLocation(Anchor + RandomOffset);

    AttributeSet->SetHealth(AttributeSet->GetMaxHealth());
    bIsDead = false;
    SetActorEnableCollision(true);
    GetMesh()->SetVisibility(true);

    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ADummyCharacter::PerformAutoAttack, AttackInterval, true);
}

void ADummyCharacter::PerformAutoAttack()
{
    if (bIsDead || !AutoAttackAbility || !AbilitySystemComponent) return;
    //AbilitySystemComponent->TryActivateAbilityByClass(AutoAttackAbility);
    bool bActivated = AbilitySystemComponent->TryActivateAbilityByClass(AutoAttackAbility);
    UE_LOG(LogTemp, Warning, TEXT("AutoAttack Activated: %s"), bActivated ? TEXT("YES") : TEXT("NO"));
}

void ADummyCharacter::PushHealthToHUD()
{
    ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (!PlayerChar) return;
    if (ACombatSystemDemoPlayerController* PC = Cast<ACombatSystemDemoPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
    {
        if (PC->HUDWidget)
            PC->HUDWidget->UpdateDummyHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
    }
}