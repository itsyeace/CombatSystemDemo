#pragma once
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class COMBATSYSTEMDEMO_API UBaseAttributeSet : public UAttributeSet
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)

        UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Damage)

    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
