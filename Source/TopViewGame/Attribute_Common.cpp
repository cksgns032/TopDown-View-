// Fill out your copyright notice in the Description page of Project Settings.

#include "Net/UnrealNetwork.h"
#include "Attribute_Common.h"
#include "GameplayEffectExtension.h"
#include "BaseMonster.h"

UAttribute_Common::UAttribute_Common()
{
}

void UAttribute_Common::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (ABaseMonster* Monster = Cast<ABaseMonster>(Data.Target.AbilityActorInfo->AvatarActor.Get()))
		{
			Monster->UpdateHpWidget();

			if (GetHealth() <= 0.0f)
			{
				Monster->Die();
			}
		}
	}
}

// �������� �޴� ���� �����ؼ� �����ϱ� ���� �Լ�
void UAttribute_Common::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAttribute_Common, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttribute_Common, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UAttribute_Common::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttribute_Common, MaxHealth, OldHealth);
	if (ABaseMonster* Monster = Cast<ABaseMonster>(GetOwningActor()))
	{
		Monster->UpdateHpWidget();
	}
}

void UAttribute_Common::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttribute_Common, Health, OldHealth);

	if (ABaseMonster* Monster = Cast<ABaseMonster>(GetOwningActor()))
	{
		Monster->UpdateHpWidget();
	}
}
