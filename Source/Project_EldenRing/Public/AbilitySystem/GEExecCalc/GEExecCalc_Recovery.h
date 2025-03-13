// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_Recovery.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UGEExecCalc_Recovery : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEExecCalc_Recovery();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};