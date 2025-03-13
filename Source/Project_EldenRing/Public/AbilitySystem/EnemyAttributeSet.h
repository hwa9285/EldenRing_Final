// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "EnemyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UEnemyAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	UEnemyAttributeSet();

	protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
