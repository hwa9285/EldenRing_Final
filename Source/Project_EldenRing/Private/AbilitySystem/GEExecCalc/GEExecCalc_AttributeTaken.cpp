// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_AttributeTaken.h"

#include "AbilitySystem/BaseAttributeSet.h"

struct FAttributeCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack)

	DECLARE_ATTRIBUTE_CAPTUREDEF(TakenBleedLev)
	DECLARE_ATTRIBUTE_CAPTUREDEF(TakenBurnLev)
	DECLARE_ATTRIBUTE_CAPTUREDEF(TakenDebuffAtkLev)
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(BleedTaken)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BurnTaken)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DebuffAtkTaken)
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence)
	
	FAttributeCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Attack, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,TakenBleedLev,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,TakenBurnLev,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, TakenDebuffAtkLev, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,BleedTaken,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,BurnTaken,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DebuffAtkTaken, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Defence, Target, false);
		
	}
};

static const FAttributeCapture& GetAttributeCapture()
{
	static FAttributeCapture AttributeCapture;
	return AttributeCapture;
}


UGEExecCalc_AttributeTaken::UGEExecCalc_AttributeTaken()
{
	RelevantAttributesToCapture.Add(GetAttributeCapture().AttackDef);

	RelevantAttributesToCapture.Add(GetAttributeCapture().TakenBleedLevDef);
	RelevantAttributesToCapture.Add(GetAttributeCapture().TakenBurnLevDef);
	RelevantAttributesToCapture.Add(GetAttributeCapture().TakenDebuffAtkLevDef);
	
	RelevantAttributesToCapture.Add(GetAttributeCapture().BleedTakenDef);
	RelevantAttributesToCapture.Add(GetAttributeCapture().BurnTakenDef);
	RelevantAttributesToCapture.Add(GetAttributeCapture().DebuffAtkTakenDef);
	
	RelevantAttributesToCapture.Add(GetAttributeCapture().DefenceDef);
}

void UGEExecCalc_AttributeTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeCapture().AttackDef, EvaluateParameters, SourceAttack);
	
	float BaseBleed = 0.f;
	float BaseBurn = 0.f;
	float BaseDebuffAtk = 0.f;
	
	float BurnLev = 0.f;
	float BleedLev = 0.f;
	float DebuffAtkLev = 0.f;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		//상태이상 적용 레벨
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_BurnLev))
		{
			BurnLev = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_BleedLev))
		{
			BleedLev = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_DebuffAtkLev))
		{
			DebuffAtkLev = TagMagnitude.Value;
		}
		
		//상태이상 누적치 추가
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_Bleed))
		{
			BaseBleed = TagMagnitude.Value;
			//UE_LOG(LogTemp, Warning, TEXT("BaseBleed: %f"), BaseBleed);
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_Burn))
		{
			BaseBurn = TagMagnitude.Value;
			//UE_LOG(LogTemp, Warning, TEXT("BaseBurn: %f"), BaseBurn);
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_DebuffAtk))
		{
			BaseDebuffAtk = TagMagnitude.Value;
			//UE_LOG(LogTemp, Warning, TEXT("BaseDebuffAtk: %f"), BaseDebuffAtk);
		}
	}
	
	float TargetDefence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeCapture().DefenceDef, EvaluateParameters, TargetDefence);

	//UE_LOG(LogTemp, Warning, TEXT("BleedLev: %f,BurnLev: %f,DebuffAtkLev: %f"), BleedLev,BurnLev,DebuffAtkLev);

	const float FinalBleed = BaseBleed * SourceAttack / TargetDefence;
	const float FinalBurn = BaseBurn * SourceAttack / TargetDefence;
	const float FinalDebuffAtk = BaseDebuffAtk * SourceAttack / TargetDefence;

	//UE_LOG(LogTemp, Warning, TEXT("FinalBleed: %f,FinalBurn: %f,FinalDebuffAtk: %f"), FinalBleed,FinalBurn,FinalDebuffAtk);
	
	if (FinalBleed > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetAttributeCapture().BleedTakenProperty,
				EGameplayModOp::Override,
				FinalBleed)
				);
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(GetAttributeCapture().TakenBleedLevProperty,
				EGameplayModOp::Override,
				BleedLev));
	}
	if (FinalBurn > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetAttributeCapture().BurnTakenProperty,
				EGameplayModOp::Override,
				FinalBurn)
				);
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(GetAttributeCapture().TakenBurnLevProperty,
				EGameplayModOp::Override,
				BurnLev));
	}
	if (FinalDebuffAtk > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetAttributeCapture().DebuffAtkTakenProperty,
				EGameplayModOp::Override,
				FinalDebuffAtk)
				);
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(GetAttributeCapture().TakenDebuffAtkLevProperty,
				EGameplayModOp::Override,
				DebuffAtkLev));
	}
}
