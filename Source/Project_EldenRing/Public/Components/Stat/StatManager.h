// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/BaseEnum.h"
#include "UObject/NoExportTypes.h"
#include "StatManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UStatManager : public UObject
{
	GENERATED_BODY()

public:
	// 특정 스탯이 Attribute에 미치는 영향을 계산 (일반화된 함수)
	float ApplyStatModifiers(EAttributeName AttributeName, const TMap<EStatsType, uint8>& BaseStats, const TMap<EStatsType, uint8>& PendingStats);

	//소모 재화 계산함수
	int32 CalculateRuneCostForUpgrade(const uint8 CurrentLevel,const uint8 NewLevel);
	
};
