// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/PlayerAttributeSet.h"

struct FDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LeftHandAttack)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Impulse)
	DECLARE_ATTRIBUTE_CAPTUREDEF(GroggyTaken)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	
	FDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet,LeftHandAttack,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Defence, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,Impulse,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,GroggyTaken,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DamageTaken, Target, false);
	}
};

static const FDamageCapture& GetDamageCapture()
{
	static FDamageCapture DamageCapture;
	return DamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	//Define을 이용한 캡처주입 방식
	RelevantAttributesToCapture.Add(GetDamageCapture().AttackDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().LeftHandAttackDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().DefenceDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().ImpulseDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().GroggyTakenDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().AttackDef, EvaluateParameters, SourceAttack);

	float SourceLeftAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().LeftHandAttackDef, EvaluateParameters, SourceLeftAttack);

	float SourceImpulse = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().ImpulseDef, EvaluateParameters, SourceImpulse);
	
	float BaseDamage = 0.f;
	float BaseImpulse = 0.f;

	int32 CachedComboCount = 0;
	float Magic_Weight = 0.f;
	float CachedChargedTime = 1.f;
	bool bIsLeft = false;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		//공용으로 사용할 데미지 가중치(커브테이블에 지정)
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		//공용으로 사용할 그로기수치(커브테이블에 지정)
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Shared_SetByCaller_Impulse))
		{
			BaseImpulse = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Player_SetByCaller_AttackType_Right_Melee))
		{
			CachedComboCount = TagMagnitude.Value;
			bIsLeft = false;
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Player_SetByCaller_AttackType_Right_Magic))
		{
			Magic_Weight = TagMagnitude.Value;
			bIsLeft = false;
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Player_SetByCaller_AttackType_Left_Melee))
		{
			CachedComboCount = TagMagnitude.Value;
			bIsLeft = true;
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Player_SetByCaller_AttackType_Left_Magic))
		{
			Magic_Weight = TagMagnitude.Value;
			bIsLeft = true;
		}
		if (TagMagnitude.Key.MatchesTagExact(BaseGamePlayTags::Player_SetByCaller_AttackType_Charged))
		{
			CachedChargedTime += TagMagnitude.Value;
		}
	}

	float TargetDefence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().DefenceDef, EvaluateParameters, TargetDefence);

	if (CachedComboCount !=0)
	{
		BaseDamage *= (CachedComboCount - 1) * 0.05 + 1.f;
	}
	if (Magic_Weight != 0.f)
	{
		BaseDamage *= Magic_Weight;
	}
	
	float FinalDamage = 0.f;
	
	if (bIsLeft)
	{
		FinalDamage = BaseDamage * SourceLeftAttack * CachedChargedTime/ TargetDefence;	
	}
	else
	{
		FinalDamage =BaseDamage * SourceAttack * CachedChargedTime/ TargetDefence;
	}
	
	const float FinalImpulse = BaseImpulse * SourceImpulse / TargetDefence;

	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamage)
				);
	}
	if (FinalImpulse > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetDamageCapture().GroggyTakenProperty,
				EGameplayModOp::Override,
				FinalImpulse)
				);
	}
}
