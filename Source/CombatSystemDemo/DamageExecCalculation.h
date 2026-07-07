#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecCalculation.generated.h"

UCLASS()
class COMBATSYSTEMDEMO_API UDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()
public:
    UDamageExecCalculation();
    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
