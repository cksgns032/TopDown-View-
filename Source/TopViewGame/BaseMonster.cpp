// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystemComponent.h"
#include "Attribute_Common.h"
#include "Components/WidgetComponent.h"
#include "../Widget_Hp.h"
#include "BaseMonster.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseMonster::ABaseMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAttribute_Common>(TEXT("AttributeSet"));

	// GetUserWidgetObject()는 위젯이 아직 생성되기 전인 생성자 시점에는 항상 null을 반환하므로
	// (UnsafeDuringActorConstruction), 여기서는 컴포넌트 배치만 하고 위젯 캐싱은 BeginPlay에서 한다.
	Wiget_HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	Wiget_HpBar->SetupAttachment(GetRootComponent());
	Wiget_HpBar->SetWidgetSpace(EWidgetSpace::Screen);
}

UAbilitySystemComponent* ABaseMonster::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ABaseMonster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseMonster, bIsDead);
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystem)
	{
		// 생성자에서 캐싱한 포인터가 어긋난 경우(예: AbilitySystem이 추가되기 전에 저장된 배치 액터)를 대비해
		// 실제 액터에 붙어있는 컴포넌트를 다시 찾아본다.
		AbilitySystem = FindComponentByClass<UAbilitySystemComponent>();
	}

	if(AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

	if (Wiget_HpBar)
	{
		HpCom = Cast<UWidget_Hp>(Wiget_HpBar->GetUserWidgetObject());
	}

	GrantAbilities();
	InitializeStats();
	UpdateHpWidget();
}

void ABaseMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABaseMonster::InitializeStats()
{
	if (!HasAuthority()) return;

	FMonsterStatRow Row;
	if (!GetMonsterStatRow(Row) || !AbilitySystem || !StatInitEffect)
	{
		return;
	}

	MaxHealth = FMath::RoundToInt(Row.HP);

	FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(StatInitEffect, 1.0f, ContextHandle);
	if (SpecHandle.IsValid())
	{
		// GE_StatInit 안에서 Health Modifier가 "Stat.Hp" SetByCaller 태그를 사용하도록 되어 있어야 한다.
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Stat.MaxHp")), static_cast<float>(MaxHealth));
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Stat.Hp")), static_cast<float>(MaxHealth));
		AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

bool ABaseMonster::GetMonsterStatRow(FMonsterStatRow& OutRow)
{
	if (!MonseterData)
	{
		if (!MonsterStatTable)
		{
			return false;
		}

		static const FString ContextString(TEXT("MonsterStatContext"));
		MonseterData = MonsterStatTable->FindRow<FMonsterStatRow>(MonsterStatRowName, ContextString);
		if (!MonseterData)
		{
			return false;
		}
	}

	OutRow = *MonseterData;
	return true;
}

void ABaseMonster::GrantAbilities()
{
	if (!HasAuthority()) return;

	if (!AbilitySystem) return;

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : GrantedAbilities)
	{
		if (!AbilityClass) continue;

		AbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
	}
}

void ABaseMonster::Die_Implementation()
{
	bIsDead = true;

	GetCharacterMovement()->DisableMovement();
	SetActorEnableCollision(false);
}

void ABaseMonster::OnRep_IsDead()
{
	Die();
}

void ABaseMonster::UpdateHpWidget()
{
	if (!HpCom && Wiget_HpBar)
	{
		HpCom = Cast<UWidget_Hp>(Wiget_HpBar->GetUserWidgetObject());
	}

	if (!HpCom || !AttributeSet) return;

	HpCom->SetHpPercent(FMath::RoundToInt(AttributeSet->GetHealth()), AttributeSet->GetMaxHealth());
}

// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// GameplayEffect 적용은 서버(권한)에서만 하고, 위젯 갱신/사망 처리는
	// Attribute_Common::PostGameplayEffectExecute에서 담당한다.
	if (!HasAuthority() || bIsDead || ActualDamage <= 0.0f || !AbilitySystem || !HitEffect)
	{
		return ActualDamage;
	}

	FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
	ContextHandle.AddSourceObject(DamageCauser ? DamageCauser : this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(HitEffect, 1.0f, ContextHandle);
	if (SpecHandle.IsValid())
	{
		// HitEffect 안에서 Health Modifier(Operation=Add)가 "State.Hit" SetByCaller 태그를 사용하도록 되어 있어야 한다.
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("State.Hit")), -ActualDamage);

		AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	return ActualDamage;
}

