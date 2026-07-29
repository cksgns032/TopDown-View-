// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterStatRow.generated.h"

class UAnimMontage;

// DT_Monster_Stat의 Row Structure를 이 구조체로 지정하면(에디터에서 변경),
// 기존 BPStruct_Monster_Stat과 필드 이름/타입이 같으므로 데이터가 그대로 유지된다.
USTRUCT(BlueprintType)
struct FMonsterStatRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	FName UniqeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	double HP = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	double Damage = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	double Defence = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	double AttackCoolTime = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	double AttackRange = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	TArray<int32> DropItmeNumbers;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	//TSoftObjectPtr<UAnimMontage> AttackMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	//TSoftObjectPtr<UAnimMontage> HitMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	//TSoftObjectPtr<UAnimMontage> DeadMontage;
};
