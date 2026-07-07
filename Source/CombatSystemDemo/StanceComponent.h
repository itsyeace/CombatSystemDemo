#pragma once
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StanceComponent.generated.h"

UENUM(BlueprintType)
enum class EStance : uint8
{
    Melee,
    Ranged
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMBATSYSTEMDEMO_API UStanceComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category = "Stance")
    EStance CurrentStance = EStance::Melee;

    UFUNCTION(BlueprintCallable, Category = "Stance")
    void SetStance(EStance NewStance) { CurrentStance = NewStance; }

    UFUNCTION(BlueprintCallable, Category = "Stance")
    bool IsMelee() const { return CurrentStance == EStance::Melee; }
};
