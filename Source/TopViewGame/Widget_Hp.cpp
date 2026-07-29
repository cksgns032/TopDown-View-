// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Hp.h"
#include "Components/ProgressBar.h"

void UWidget_Hp::SetHpPercent(int32 Cur, int32 Max)
{
	if (!HP_Progress) return;

	const float Percent = Max > 0 ? static_cast<float>(Cur) / static_cast<float>(Max) : 0.0f;
	HP_Progress->SetPercent(Percent);
}

