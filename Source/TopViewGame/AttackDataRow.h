// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AttackDataRow.generated.h"

class UAnimMontage;

// 플레이어 공격 콤보 / 스킬, 몬스터 공격 공용 판정 데이터.
// RowName을 공격/스킬 식별자로 사용한다 (예: Player_Combo_01, Skill_Fireball, Golem_Attack_01).
USTRUCT(BlueprintType)
struct FAttackDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 UniqueID = 0;

	// 캐릭터 정면 기준 판정 시작 거리 (AHitBox::DoBoxTrace의 Distance)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	double AttackDistance = 0.0;

	// 판정 박스 크기 (AHitBox::DoBoxTrace의 Size)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FVector AttackSize = FVector(50.0, 50.0, 50.0);

	// 이 공격/스킬 판정과 함께 재생할 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	TSoftObjectPtr<UAnimMontage> AttackMontage;

	// 몽타주 내에서 재생을 시작할 섹션 (콤보 단계 구분용, 비워두면 처음부터 재생)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FName MontageStartSection;

	// 기본 Damage(FMonsterStatRow::Damage 등)에 곱해지는 공격 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	double DamageMultiplier = 1.0;

	// 한 번의 판정으로 동시에 맞을 수 있는 최대 대상(인원) 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 MaxTargetCount = 1;
};
