// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/BaseEnum.h"
#include "Components/ActorComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "StatComponent.generated.h"

class IPawnUIInterface;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRuneChanged, float, NewRune);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPreviewRuneCost,int32,CostRune,int32,CurrentRune);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged,EStatsType,StatType,uint8,Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributePreviewDelegate,EAttributeName,AttributeType,float,PreviewValue,bool,bIsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateAttack,float,NewAttack);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_ELDENRING_API UStatComponent : public UPawnExtensionComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	TMap<EStatsType,uint8> BaseStat;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	TMap<EStatsType,uint8> PendingStats;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	TMap<EAttributeName, float> CurrentAttribute;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	TMap<EAttributeName,float> ExpectedAttribute;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	uint8 CurrentLevel = 1;
	
	//올라가는 레벨
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	uint8 PreviewLevel = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	int32 RightWeaponAttack;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	int32 LeftWeaponAttack;
	
	//소유 룬
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	int32 PlayerRune;
	
	//예상 소모 룬
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	int32 NextLevelCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 PreviewTotalCost = 0;    // 현재까지 PendingLevel로 올릴 때 총 Rune 소비량


	UFUNCTION(BlueprintCallable)
	void SetCurrentArrtibute();
	
	//현재 Attribute값을 가져오는 함수
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetCurrentAttribute(EAttributeName AttributeNames);

	// 스탯 증가 (임시 적용)
	UFUNCTION(BlueprintCallable)
	void IncreaseStatTemporarily(EStatsType StatName, uint8 Amount);

	//스탯 감소
	UFUNCTION(BlueprintCallable)
	void DecreaseStatTemporarily(EStatsType StatName, uint8 Amount);
	
	// 예상 Attribute 값 계산
	UFUNCTION(BlueprintCallable)
	void CalculateExpectedAttributes();

	// 스탯 확정
	UFUNCTION(BlueprintCallable)
	void ConfirmStats();

	// 변경 취소
public:
	UFUNCTION(BlueprintCallable)
	void ResetPendingStats();

	UFUNCTION(BlueprintCallable)
	TMap<EStatsType,uint8> GetBaseStats(){return BaseStat;}

	UFUNCTION(BlueprintCallable)
	uint8 GetCurrentLevel(){return CurrentLevel;}

	UFUNCTION(BlueprintCallable)
	void SetBaseStats(TMap<EStatsType,uint8> SaveData){BaseStat = SaveData;}

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevel(uint8 SaveData){CurrentLevel = SaveData;};

public:
	UPROPERTY(BlueprintAssignable,Category="Stat")
	FOnStatChanged OnStatChanged;

	UPROPERTY(BlueprintAssignable,Category="Stat")
	FOnAttributePreviewDelegate OnAttributePreviewDelegate;

	UPROPERTY(BlueprintAssignable,Category="Stat")
	FOnPlayerRuneChanged OnPlayerRuneChanged;

	UPROPERTY(BlueprintAssignable,Category="Stat")
	FOnPreviewRuneCost OnPreviewRuneCost;

	UPROPERTY(BlueprintAssignable,Category="Stat")
	FOnUpdateAttack OnUpdateAttack;

	TWeakInterfacePtr<IPawnUIInterface> GetPawnUIInterface(){return CachedUIInterface;};

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedUIInterface;
};