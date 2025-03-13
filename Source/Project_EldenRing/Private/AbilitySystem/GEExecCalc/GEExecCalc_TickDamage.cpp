// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_TickDamage.h"

#include "AbilitySystem/BaseAttributeSet.h"


struct FAttributeLevCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	FAttributeLevCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Defence, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DamageTaken, Source, false);
	}
};
static const FAttributeLevCapture& GetTickDamageCapture()
{
	static FAttributeLevCapture AttributeLevCapture;
	return AttributeLevCapture;
}

UGEExecCalc_TickDamage::UGEExecCalc_TickDamage()
{
	RelevantAttributesToCapture.Add(GetTickDamageCapture().DefenceDef);
	RelevantAttributesToCapture.Add(GetTickDamageCapture().DamageTakenDef);
}

void UGEExecCalc_TickDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	
	float SourceDefence;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetTickDamageCapture().DefenceDef,EvaluateParameters,SourceDefence);
	
	
	float SetByCallerTickDamage = 0.f;
	
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_TickDamage))
		{
			SetByCallerTickDamage = TagMagnitude.Value;
		}
	}
	
	float FinalDamage = SetByCallerTickDamage/SourceDefence;
	
	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetTickDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamage)
		);
	}
}
