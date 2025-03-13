// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Stat/StatComponent.h"

#include "BaseGameInstance.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/PlayerAttributeSet.h"
#include "Character/PlayerCharacter.h"
#include "Components/Stat/StatManager.h"
#include "SubSystem/StatSubsystem.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	BaseStat.Empty();
	BaseStat.Add(EStatsType::Strength, 10);
	BaseStat.Add(EStatsType::Intellect, 5);
	BaseStat.Add(EStatsType::Agility, 10);
	BaseStat.Add(EStatsType::Endurance, 10);
	BaseStat.Add(EStatsType::Faith, 10);
	BaseStat.Add(EStatsType::Mentality, 10);
	BaseStat.Add(EStatsType::Mystery, 10);
	BaseStat.Add(EStatsType::Vitality, 10);
	BaseStat.Add(EStatsType::Level, 1);

	PendingStats.Empty();
	PendingStats = BaseStat;
	
	UStatSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStatSubsystem>();
	UStatManager* StatManager = Subsystem->StatManager;
	if (!StatManager) return;
	
	NextLevelCost = StatManager->CalculateRuneCostForUpgrade(CurrentLevel,PreviewLevel);
}

void UStatComponent::SetCurrentArrtibute()
{
	if (AActor* Owner = GetOwningPawn<APawn>())
	{
		if (UAbilitySystemComponent* ASC = Owner->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (const UPlayerAttributeSet* Attributes = Cast<UPlayerAttributeSet>(ASC->GetAttributeSet(UPlayerAttributeSet::StaticClass())))
			{
				CurrentAttribute.Add(EAttributeName::MaxHp, Attributes->GetMaxHp());
				CurrentAttribute.Add(EAttributeName::MaxMp, Attributes->GetMaxFP());
				CurrentAttribute.Add(EAttributeName::MaxStamina, Attributes->GetMaxStamina());

				CurrentAttribute.Add(EAttributeName::BaseAttack,Attributes->GetBaseAttack());
				
				//표시용
				LeftWeaponAttack = Attributes->GetLeftWeaponAttack();
				RightWeaponAttack = Attributes->GetRightWeaponAttack();
				
				//CurrentAttribute.Add(EAttributeName::Attack, Attributes->GetAttack());
				//CurrentAttribute.Add(EAttributeName::LeftAttack, Attributes->GetLeftHandAttack());
				
				CurrentAttribute.Add(EAttributeName::Defence, Attributes->GetDefence());

				PlayerRune = Attributes->GetRune();
			}
		}
	}
}

float UStatComponent::GetCurrentAttribute(EAttributeName AttributeNames)
{
	if (AActor* Owner = GetOwningPawn<APawn>())
	{
		if (UAbilitySystemComponent* ASC = Owner->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (const UPlayerAttributeSet* Attributes = Cast<UPlayerAttributeSet>(ASC->GetAttributeSet(UPlayerAttributeSet::StaticClass())))
			{
				CurrentAttribute.Add(EAttributeName::MaxHp, Attributes->GetMaxHp());
				CurrentAttribute.Add(EAttributeName::MaxMp, Attributes->GetMaxFP());
				CurrentAttribute.Add(EAttributeName::MaxStamina, Attributes->GetMaxStamina());

				CurrentAttribute.Add(EAttributeName::BaseAttack,Attributes->GetBaseAttack());

				LeftWeaponAttack = Attributes->GetLeftWeaponAttack();
				RightWeaponAttack = Attributes->GetRightWeaponAttack();
				
				CurrentAttribute.Add(EAttributeName::Defence, Attributes->GetDefence());

				PlayerRune = Attributes->GetRune();
			}
		}
	}
	if (AttributeNames == EAttributeName::Attack)
	{
		return *CurrentAttribute.Find(EAttributeName::BaseAttack) + RightWeaponAttack;
	}
	if (AttributeNames == EAttributeName::LeftAttack)
	{
		return *CurrentAttribute.Find(EAttributeName::BaseAttack) + LeftWeaponAttack;
	}
	return *CurrentAttribute.Find(AttributeNames);
}

//예상 Attribute UI반영
void UStatComponent::IncreaseStatTemporarily(EStatsType StatName, uint8 Amount)
{
	UStatSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStatSubsystem>();
	UStatManager* StatManager = Subsystem->StatManager;
	
	if (!StatManager) return;

	PreviewLevel += 1;
	
	if (PendingStats.Contains(StatName))
	{
		UE_LOG(LogTemp,Error,TEXT(" StatName %s : current Value: %d"),*UEnum::GetValueAsString(StatName), PendingStats[StatName]);
		PendingStats[StatName] += Amount;
		UE_LOG(LogTemp,Error,TEXT(" StatName %s : pending Value: %d"),*UEnum::GetValueAsString(StatName), PendingStats[StatName]);
	}
	
	CalculateExpectedAttributes();
	OnStatChanged.Broadcast(StatName,PendingStats[StatName]);
	
	PreviewTotalCost = StatManager->CalculateRuneCostForUpgrade(CurrentLevel, PreviewLevel);
    
	// PendingLevel에서 한 단계 더 올릴 때 필요한 추가 비용
	NextLevelCost = StatManager->CalculateRuneCostForUpgrade(PreviewLevel, PreviewLevel + 1);
	OnPreviewRuneCost.Broadcast(NextLevelCost, PlayerRune - PreviewTotalCost);
}

void UStatComponent::DecreaseStatTemporarily(EStatsType StatName, uint8 Amount)
{
	UStatSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStatSubsystem>();
	UStatManager* StatManager = Subsystem->StatManager;
	
	if (!StatManager) return;

	PreviewLevel -= 1;

	if (PendingStats.Contains(StatName))
	{
		PendingStats[StatName] -= Amount;
	}

	PreviewTotalCost = StatManager->CalculateRuneCostForUpgrade(CurrentLevel, PreviewLevel);
	NextLevelCost = StatManager->CalculateRuneCostForUpgrade(PreviewLevel, PreviewLevel + 1);

	CalculateExpectedAttributes();
	OnStatChanged.Broadcast(StatName,PendingStats[StatName]);
	
	OnPreviewRuneCost.Broadcast(NextLevelCost, PlayerRune - PreviewTotalCost);
}

//감소 로직도 필요함.

//예상 Attribute 계산
void UStatComponent::CalculateExpectedAttributes()
{
	UStatSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStatSubsystem>();
	UStatManager* StatManager = Subsystem->StatManager;

	if (!StatManager) return;
	
	//예상값
	//만약, PendingStats의 key값이 힘이라고 칠 때, MaxHp가 Attribute로 변화되면 값이 추가될것임.
	ExpectedAttribute.Add(EAttributeName::MaxHp, StatManager->ApplyStatModifiers(EAttributeName::MaxHp,BaseStat,PendingStats));
	ExpectedAttribute.Add(EAttributeName::MaxMp, StatManager->ApplyStatModifiers(EAttributeName::MaxMp,BaseStat,PendingStats));

	ExpectedAttribute.Add(EAttributeName::BaseAttack,StatManager->ApplyStatModifiers(EAttributeName::Attack,BaseStat,PendingStats));
	
	//ExpectedAttribute.Add(EAttributeName::Attack, StatManager->ApplyStatModifiers(EAttributeName::Attack,BaseStat,PendingStats));
	//ExpectedAttribute.Add(EAttributeName::LeftAttack, StatManager->ApplyStatModifiers(EAttributeName::Attack,BaseStat,PendingStats));

	ExpectedAttribute.Add(EAttributeName::Defence, StatManager->ApplyStatModifiers(EAttributeName::Defence,BaseStat,PendingStats));
	ExpectedAttribute.Add(EAttributeName::MaxStamina, StatManager->ApplyStatModifiers(EAttributeName::MaxStamina,BaseStat,PendingStats));
	ExpectedAttribute.Add(EAttributeName::Rune, - (PreviewTotalCost + NextLevelCost));
	
	for (const auto& Attribute : ExpectedAttribute)
	{
		if (Attribute.Key != EAttributeName::Rune)
		{
			float CurrentValue = CurrentAttribute[Attribute.Key];
			float ExpectedValue = Attribute.Value;
			
			if (Attribute.Key == EAttributeName::BaseAttack)
			{
				OnAttributePreviewDelegate.Broadcast(EAttributeName::Attack, CurrentValue+ExpectedValue + RightWeaponAttack,CurrentValue < CurrentValue+ExpectedValue);
				OnAttributePreviewDelegate.Broadcast(EAttributeName::LeftAttack, CurrentValue+ExpectedValue +LeftWeaponAttack,CurrentValue < CurrentValue+ExpectedValue);
			}
			else
			{
				OnAttributePreviewDelegate.Broadcast(Attribute.Key, CurrentValue+ExpectedValue, CurrentValue < CurrentValue+ExpectedValue);		
			}
		}
	}

}

//스탯 반영
void UStatComponent::ConfirmStats()
{
	BaseStat = PendingStats;  // 최종 확정
	CurrentLevel = PreviewLevel;
	
	// GAS 적용을 위한 AttributeSet 업데이트
	if (AActor* Owner = GetOwningPawn<APawn>())
	{
		if (UAbilitySystemComponent* ASC = Owner->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (const UPlayerAttributeSet* Attributes = Cast<UPlayerAttributeSet>(ASC->GetAttributeSet(UPlayerAttributeSet::StaticClass())))
			{
				//최대체력이 변하면, Current도 Max로 채우게 수정
				ASC->ApplyModToAttribute(Attributes->GetMaxHpAttribute(), EGameplayModOp::Additive,ExpectedAttribute[EAttributeName::MaxHp]);
				ASC->ApplyModToAttribute(Attributes->GetCurrentHpAttribute(),EGameplayModOp::Override,Attributes->GetMaxHp());
				
				ASC->ApplyModToAttribute(Attributes->GetMaxFPAttribute(), EGameplayModOp::Additive,ExpectedAttribute[EAttributeName::MaxMp]);
				ASC->ApplyModToAttribute(Attributes->GetCurrentFPAttribute(),EGameplayModOp::Override,Attributes->GetMaxFP());

				ASC->ApplyModToAttribute(Attributes->GetMaxStaminaAttribute(), EGameplayModOp::Additive,ExpectedAttribute[EAttributeName::MaxStamina]);
				ASC->ApplyModToAttribute(Attributes->GetCurrentStaminaAttribute(),EGameplayModOp::Override,Attributes->GetMaxStamina());
				
				ASC->ApplyModToAttribute(Attributes->GetBaseAttackAttribute(),EGameplayModOp::Additive,ExpectedAttribute[EAttributeName::BaseAttack]);
				ASC->ApplyModToAttribute(Attributes->GetDefenceAttribute(), EGameplayModOp::Additive,ExpectedAttribute[EAttributeName::Defence]);
				ASC->ApplyModToAttribute(Attributes->GetRuneAttribute(),EGameplayModOp::Additive,ExpectedAttribute[EAttributeName::Rune]);

				if (!CachedUIInterface.IsValid())
				{
					CachedUIInterface = TWeakInterfacePtr<IPawnUIInterface>(GetOwner());
				}
				
				check(CachedUIInterface.IsValid());
				if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
				{
					PlayerUIComponent->OnMaxHpChanged.Broadcast(Attributes->GetMaxHp());
					PlayerUIComponent->OnMaxStaminaChanged.Broadcast(Attributes->GetMaxStamina());
					PlayerUIComponent->OnMaxFPChanged.Broadcast(Attributes->GetMaxFP());
					PlayerUIComponent->OnCurrentHPChanged.Broadcast(1);
					PlayerUIComponent->OnCurrentStaminaChanged.Broadcast(1);
					PlayerUIComponent->OnCurrentFPChanged.Broadcast(1);
				}
			}
		}
	}
	
	//초기화 작업
	ResetPendingStats();
}

void UStatComponent::ResetPendingStats()
{
	PendingStats = BaseStat;
    
	// PreviewLevel은 이제 실제 레벨(CurrentLevel)과 같아집니다.
	PreviewLevel = CurrentLevel;
    
	// 임시 룬 소비 관련 변수 초기화
	PreviewTotalCost = 0;
    
	// 게임 인스턴스에서 StatManager를 통해 다음 레벨업 비용을 계산합니다.
	UStatSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStatSubsystem>();
	UStatManager* StatManager = Subsystem->StatManager;
	if (StatManager)
	{
		// 현재 레벨에서 다음 레벨업 시 필요한 비용 계산 (예: CurrentLevel -> CurrentLevel+1)
		NextLevelCost = StatManager->CalculateRuneCostForUpgrade(CurrentLevel, CurrentLevel + 1);
	}
    
	// 확정 후 플레이어 Attribute를 다시 가져와 최신값으로 업데이트
	SetCurrentArrtibute();
	// 이 때, PlayerRune이 최신 값으로 업데이트
    
	// UI 업데이트: 룬 프리뷰 (예: "다음 레벨업 비용 / 남은 룬")
	OnPreviewRuneCost.Broadcast(NextLevelCost, PlayerRune);
    
	// UI 업데이트: 각 Attribute 프리뷰를 초기 상태(확정된 값)로 표시  
	// 예: OnAttributePreviewDelegate(속성, 현재값, bIsChanged)
	for (const auto& AttrPair : CurrentAttribute)
	{
		if (AttrPair.Key == EAttributeName::BaseAttack)
		{
			OnAttributePreviewDelegate.Broadcast(EAttributeName::Attack, AttrPair.Value + RightWeaponAttack,false);
			OnAttributePreviewDelegate.Broadcast(EAttributeName::LeftAttack, AttrPair.Value +LeftWeaponAttack,false);
		}
		else
		{
			OnAttributePreviewDelegate.Broadcast(AttrPair.Key, AttrPair.Value, false);		
		}
		// bIsChanged를 false로 전달하여 색상 등 변경 표시가 없게
	}
	for (const auto& StatPair : BaseStat)
	{
		OnStatChanged.Broadcast(StatPair.Key, StatPair.Value);
	}
	
}







