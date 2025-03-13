// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPlayerAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
public:
	UPlayerAttributeSet();
	
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData MaxFP;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,MaxFP);
	
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData CurrentFP;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,CurrentFP);
	
	UPROPERTY(BlueprintReadOnly, Category="Player")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category="Player")
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,CurrentStamina);

	//왼손 최종 공격력
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData LeftHandAttack;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,LeftHandAttack);

	//왼손 무기 공격력
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData LeftWeaponAttack;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,LeftWeaponAttack);

	//오른손 무기 공격력
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData RightWeaponAttack;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,RightWeaponAttack);

	//기본 공격력
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData BaseAttack;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,BaseAttack);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData RightHandCostStamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,RightHandCostStamina);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData LeftHandCostStamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,LeftHandCostStamina);

protected:
	// 스탯이 변경될 때 기본 능력치 업데이트
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
};

