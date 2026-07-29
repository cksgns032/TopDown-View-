// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Attribute_Common.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class TOPVIEWGAME_API UAttribute_Common : public UAttributeSet
{
	GENERATED_BODY()

public:
    UAttribute_Common();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // GE 적용 직후 서버(권한)에서 바로 호출된다. OnRep_Health는 클라이언트에서만 호출되므로,
    // 서버 쪽에서도 동일한 처리가 필요하면 여기서 실행해야 한다.
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UAttribute_Common, MaxHealth)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UAttribute_Common, Health)

protected:
    UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldHealth);

    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	
};
