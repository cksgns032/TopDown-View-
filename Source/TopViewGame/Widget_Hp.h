// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Hp.generated.h"

class UProgressBar;

/**
 *
 */
UCLASS()
class TOPVIEWGAME_API UWidget_Hp : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 블루프린트 디자이너에서 이름이 "HpProgressBar"인 Progress Bar를 추가해야 자동으로 연결된다.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> HP_Progress;

	UFUNCTION(BlueprintCallable, Category = "HP")
	void SetHpPercent(int32 Cur, int32 Max);
};
