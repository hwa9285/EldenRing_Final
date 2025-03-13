// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class IPawnUIInterface;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category="Shared")
	FGameplayAttributeData MaxHp;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,MaxHp);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData CurrentHp;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,CurrentHp);
	
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Attack);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Defence);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData Impulse;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Impulse);
	
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData MaxGroggy;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,MaxGroggy);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData CurrentGroggy;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,CurrentGroggy);
	
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData Rune;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Rune);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, DamageTaken)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData GroggyTaken;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, GroggyTaken)

	//상태이상
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData CurrentBleed;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,CurrentBleed);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData MaxBleed;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,MaxBleed);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData CurrentAtkDeBuff;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,CurrentAtkDeBuff);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData MaxAtkDeBuff;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,MaxAtkDeBuff);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData CurrentBurn;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,CurrentBurn);

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData MaxBurn;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,MaxBurn);

	//Taken
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BurnTaken;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BurnTaken)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DebuffAtkTaken;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, DebuffAtkTaken)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BleedTaken;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BleedTaken)
	
	//속성레벨
	UPROPERTY(BlueprintReadOnly, Category = "AttributeLev")
	FGameplayAttributeData TakenBleedLev;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, TakenBleedLev)

	UPROPERTY(BlueprintReadOnly, Category = "AttributeLev")
	FGameplayAttributeData TakenBurnLev;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, TakenBurnLev)
	
	UPROPERTY(BlueprintReadOnly, Category = "AttributeLev")
	FGameplayAttributeData TakenDebuffAtkLev;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, TakenDebuffAtkLev)
	
	TWeakInterfacePtr<IPawnUIInterface> GetPawnUIInterface(){return CachedUIInterface;};

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedUIInterface;

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
};
