// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Stat/StatManager.h"

#include "SubSystem/StatSubsystem.h"
#include "WorldPartition/Cook/WorldPartitionCookPackage.h"


float UStatManager::ApplyStatModifiers(EAttributeName AttributeName, const TMap<EStatsType, uint8>& BaseStats, const TMap<EStatsType, uint8>& PendingStats)
{
	float TotalResult = 0.0f;

	for (const auto& Pair : PendingStats)
	{
		EStatsType StatType = Pair.Key;
		uint8 NewValue = Pair.Value;

		// BaseStat에 저장된 기존 값 (없으면 0으로 처리)
		uint8 OldValue = 0;
		if (const uint8* Found = BaseStats.Find(StatType))
		{
			OldValue = *Found;
		}

		// 커브 테이블에서 stat과 attribute에 해당하는 row 이름 생성
		// (RightChop을 이용해 enum 이름에서 접두어를 제거하는 부분은
		//  enum 이름의 형식에 의존하므로, 필요하다면 별도 매핑 함수를 만드는 것이 좋습니다.)
		FString StatString = UEnum::GetValueAsString(StatType);
		FString AttributeString = UEnum::GetValueAsString(AttributeName);
		FString RowName = StatString.RightChop(12) + "." + AttributeString.RightChop(16);

		UStatSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStatSubsystem>();
		
		const FRealCurve* Curve = Subsystem->StatCurve->FindCurve(FName(*RowName), FString());
		if (Curve != nullptr)
		{
			// 만약 BaseStat의 값은 이미 적용되어 있다고 가정하고
			// 추가 증가된 단계만 누적하려면 아래와 같이 OldValue+1부터 NewValue까지 반복합니다.
			// (만약 BaseStat 단계의 값도 다시 누적해야 한다면, Level = OldValue부터 시작)
			for (uint8 Level = OldValue + 1; Level <= NewValue; ++Level)
			{
				float BonusForLevel = FMath::RoundToInt( Curve->Eval(Level));
				TotalResult += BonusForLevel;
			}
		}
	}
	return TotalResult;
}

int32 UStatManager::CalculateRuneCostForUpgrade(const uint8 CurrentLevel,const uint8 NewLevel)
{
	UStatSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStatSubsystem>();
	int32 TotalCost = 0;
	FString RowName = "Rune";
	const FRealCurve* Curve = Subsystem->CostRuneTable->FindCurve(FName(*RowName), FString());

	if(!Curve)
	{
		// Curve가 없다면 기본 비용 0 또는 에러 처리를 할 수 있음
		return 0;
	}
	
	if (CurrentLevel == NewLevel)
	{
		float CostForLevel = Curve->Eval(CurrentLevel);
		TotalCost += FMath::RoundToInt(CostForLevel);
		return TotalCost;
	}
    
	// CurrentLevel NewLevel까지 각 레벨에 해당하는 비용을 누적
	for (uint8 Level = CurrentLevel ; Level < NewLevel; ++Level)
	{
		float CostForLevel = Curve->Eval(Level);
		TotalCost += FMath::RoundToInt(CostForLevel);
	}
    
	return TotalCost;
}


