// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"

// Sets default values
AHitBox::AHitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHitBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FHitResult> AHitBox::DoBoxTrace(FVector Size, float Distance)
{
	TArray<FHitResult> HitResults;

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return HitResults;

	const FVector OwnerLocation = OwnerActor->GetActorLocation();
	const FRotator OwnerRotation = OwnerActor->GetActorRotation();

	// Owner의 정면 방향(회전 적용)으로 Distance만큼 나간 위치에서 트레이스한다.
	const FVector TraceLocation = OwnerLocation + OwnerActor->GetActorForwardVector() * Distance;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);

	UKismetSystemLibrary::BoxTraceMulti(
		this,
		TraceLocation,
		TraceLocation,
		Size,
		OwnerRotation,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), // AttackTrace (DefaultEngine.ini에 정의됨)
		false,
		ActorsToIgnore,
		bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.0f);

	return HitResults;
}

