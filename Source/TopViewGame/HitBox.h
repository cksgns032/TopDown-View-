// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitBox.generated.h"

UCLASS()
class TOPVIEWGAME_API AHitBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Size: 박스 트레이스 half extent(크기), Position: Owner 로컬 기준 오프셋.
	// Owner의 현재 회전을 적용해서 "정면 기준" 월드 위치로 변환한 뒤 그 지점에서 박스 트레이스를 수행한다.
	UFUNCTION(BlueprintCallable, Category = "HitBox")
	TArray<FHitResult> DoBoxTrace(FVector Size, float Distance);

	// true면 DoBoxTrace가 트레이스 박스와 히트 지점을 화면에 그려준다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
	bool bDrawDebugTrace = true;

};
