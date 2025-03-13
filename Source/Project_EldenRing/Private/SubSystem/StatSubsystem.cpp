// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/StatSubsystem.h"

#include "Components/Stat/StatManager.h"

UStatSubsystem::UStatSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UCurveTable> CurveTable(TEXT("/Game/_BP/Player/CurveTable/Stat_Curve.Stat_Curve"));
	static ConstructorHelpers::FObjectFinder<UCurveTable> RuneCostCurveTable(TEXT("/Game/_BP/Player/CurveTable/Rune_Cost.Rune_Cost"));
	if (CurveTable.Succeeded() && RuneCostCurveTable.Succeeded())
	{
		StatCurve = CurveTable.Object;
		CostRuneTable = RuneCostCurveTable.Object;
	}
}

void UStatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	StatManager = NewObject<UStatManager>(this, UStatManager::StaticClass());
}



