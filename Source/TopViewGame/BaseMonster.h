// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Engine/DataTable.h"
#include "MonsterStatRow.h"
#include "BaseMonster.generated.h"


class UAbilitySystemComponent;
class UAttribute_Common;
class UAnimMontage;
class UGameplayEffect;
class UGameplayAbility;
class UUserWidget;
class UWidget_Hp;
class UWidgetComponent;

UCLASS()
class TOPVIEWGAME_API ABaseMonster : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMonster();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	// DT_Monster_Stat 등, Row Structure가 FMonsterStatRow(MonsterStatRow.h)로 지정된 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> MonsterStatTable;

	// InitializeStats()에서 캐싱해두는 로우 데이터. UPROPERTY로 리플렉션 불가(UObject가 아닌 USTRUCT의
	// raw 포인터는 UHT가 지원하지 않음) - MonsterStatTable이 살아있는 동안에만 유효하다.
	FMonsterStatRow* MonseterData = nullptr;

	// 블루프린트에서 MonseterData를 참조할 수 있도록 값으로 복사해서 넘겨준다. 로우가 아직 없으면 false.
	UFUNCTION(BlueprintPure, Category = "DataTable")
	bool GetMonsterStatRow(FMonsterStatRow& OutRow);

	// 위 테이블에서 이 몬스터가 읽어올 로우 이름 (에디터에서 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	FName MonsterStatRowName;

// MONTAGE
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> AttackMontages;

// WIDGET
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UWidgetComponent> Wiget_HpBar;
	TObjectPtr<UWidget_Hp> HpCom;

	// InitializeStats()에서 캐싱해두는 최대 체력 (HP바 퍼센트 계산용)
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	int32 MaxHealth = 0;

	// Health가 바뀔 때마다 Attribute_Common(PostGameplayEffectExecute/OnRep_Health)에서 호출된다.
	void UpdateHpWidget();

//GAS
public:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TObjectPtr<UAttribute_Common> AttributeSet;

	// 몬스터 스탯 초기화에 사용할 GameplayEffect (예: GE_StatInit). 에디터에서 지정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayEffect> StatInitEffect;

	// 데미지 적용에 사용할 GameplayEffect (예: GE_Damage). Health Modifier가 "Stat.Damage" SetByCaller 태그를
	// 사용하는 Instant/Add 이펙트여야 한다. 에디터에서 지정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayEffect> HitEffect;

	// 몬스터가 시작 시 부여받는 GameplayAbility 목록. 에디터에서 지정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	void InitializeStats();

	// GrantedAbilities를 AbilitySystem에 부여한다. 서버(권한)에서만 호출해야 한다.
	void GrantAbilities();

	// 서버에서 Health <= 0이 되면 Attribute_Common::PostGameplayEffectExecute에서 직접 호출된다.
	// 클라이언트에서는 bIsDead가 복제되면서 OnRep_IsDead를 통해 호출된다.
	// 사망 연출(애니메이션, 루트 드랍 등)이 필요하면 이 함수를 오버라이드하는 블루프린트(BP_Enemy 등)에서 구현한다.
	UFUNCTION(BlueprintNativeEvent, Category = "Monster")
	void Die();
	virtual void Die_Implementation();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsDead, Category = "Monster")
	bool bIsDead = false;

protected:
	UFUNCTION()
	void OnRep_IsDead();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// UGameplayStatics::ApplyDamage / ApplyPointDamage 등 표준 데미지 파이프라인의 진입점.
	// 서버(권한)에서 DamageEffect를 자신에게 적용해 Health를 깎는다. 위젯 갱신과 사망 처리는
	// Attribute_Common::PostGameplayEffectExecute에서 일괄적으로 담당한다.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
