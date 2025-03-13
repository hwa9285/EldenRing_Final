// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_Recovery.h"

#include "AbilitySystem/PlayerAttributeSet.h"

struct FRecoveryCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHp)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxFP)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHp)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentFP)
	
	FRecoveryCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, CurrentHp, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, CurrentFP, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, MaxHp, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, MaxFP, Source, false);
	}
};
static const FRecoveryCapture& GetRecoveryCapture()
{
	static FRecoveryCapture RecoveryCapture;
	return RecoveryCapture;
}

UGEExecCalc_Recovery::UGEExecCalc_Recovery()
{
	RelevantAttributesToCapture.Add(GetRecoveryCapture().MaxHpDef);
	RelevantAttributesToCapture.Add(GetRecoveryCapture().MaxFPDef);
	RelevantAttributesToCapture.Add(GetRecoveryCapture().CurrentHpDef);
	RelevantAttributesToCapture.Add(GetRecoveryCapture().CurrentFPDef);
}

void UGEExecCalc_Recovery::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	
	float SourceMaxHp;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetRecoveryCapture().MaxHpDef,EvaluateParameters,SourceMaxHp);
	float SourceMaxFp;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetRecoveryCapture().MaxFPDef,EvaluateParameters,SourceMaxFp);
	
	
	float HpCoefficient = 0.f;
	float FpCoefficient = 0.f;
	
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Player_SetByCaller_Recovery_Fp))
		{
			FpCoefficient = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Player_SetByCaller_Recovery_Hp))
		{
			HpCoefficient = TagMagnitude.Value;
		}
	}
	
	float FinalHpRecovery = HpCoefficient*SourceMaxHp;
	float FinalFpRecovery = FpCoefficient*SourceMaxFp;

	UE_LOG(LogTemp,Warning,TEXT("HP: %f,FP: %f"),FinalHpRecovery,FinalFpRecovery);
	
	if (FinalHpRecovery > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetRecoveryCapture().CurrentHpProperty,
				EGameplayModOp::Additive,
				FinalHpRecovery)
		);
	}
	if (FinalFpRecovery > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetRecoveryCapture().CurrentFPProperty,
				EGameplayModOp::Additive,
				FinalFpRecovery)
		);
	}
}
