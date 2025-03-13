// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BaseGameplayAbility.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


void UBaseGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EBaseAbilityActivationPolcy::OnGiven)
	{
		//액터의 정보를 받아올 수 있거나 해당 어빌리티가 활성화 되지 않았으면
		if (ActorInfo && !Spec.IsActive())
		{
			//현재 가지고 있는 어빌리티를 발동시킨다.
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EBaseAbilityActivationPolcy::OnGiven)
	{
		//어빌리티가 유효한지 체크
		if (ActorInfo)
		{
			//클리어
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UBaseGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UBaseAbilitySystemComponent* UBaseGameplayAbility::GetBaseAbilitySystemComponentFromActorInfo() const
{
	return Cast<UBaseAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UBaseGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& SpecHandle)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(ASC && SpecHandle.IsValid());

	return GetBaseAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, ASC);
}

FActiveGameplayEffectHandle UBaseGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& SpecHandle, EBaseSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EBaseSuccessType::Success : EBaseSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}


FGameplayEffectSpecHandle UBaseGameplayAbility::MakeAttributeEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect,
	const FScalableFloat& BleedAmount,const FScalableFloat& BurnAmount ,const FScalableFloat& DebuffAtkAmount,const FAttributeLevels AttLevels)
{
	check(Effect);
		
	//ContextHandle을 만들어서 타겟정보 주입
	FGameplayEffectContextHandle ContextHandle;
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
   
	//SpecHandle을 만들어서 ContextHandle 정보주입
	FGameplayEffectSpecHandle SpecHandle = GetBaseAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(Effect, GetAbilityLevel(), ContextHandle);

	SpecHandle.Data->SetSetByCallerMagnitude(
		BaseGamePlayTags::Shared_SetByCaller_Bleed,
		BleedAmount.GetValueAtLevel(AttLevels.BleedLev));
	SpecHandle.Data->SetSetByCallerMagnitude(BaseGamePlayTags::Shared_SetByCaller_BleedLev,AttLevels.BleedLev);
	
	SpecHandle.Data->SetSetByCallerMagnitude(
		BaseGamePlayTags::Shared_SetByCaller_Burn,
		BurnAmount.GetValueAtLevel(AttLevels.BurnLev));
	SpecHandle.Data->SetSetByCallerMagnitude(BaseGamePlayTags::Shared_SetByCaller_BurnLev,AttLevels.BurnLev);
	
	SpecHandle.Data->SetSetByCallerMagnitude(
		BaseGamePlayTags::Shared_SetByCaller_DebuffAtk,
		DebuffAtkAmount.GetValueAtLevel(AttLevels.DebuffAtkLev));
	SpecHandle.Data->SetSetByCallerMagnitude(BaseGamePlayTags::Shared_SetByCaller_DebuffAtkLev,AttLevels.DebuffAtkLev);
	
	
	return SpecHandle;
}


FGameplayEffectSpecHandle UBaseGameplayAbility::MakeTickDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect,
                                                                               const float BaseDamage,EAttributeType Type)
{
	check(Effect);
		
	//ContextHandle을 만들어서 타겟정보 주입
	FGameplayEffectContextHandle ContextHandle;
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
   
	//SpecHandle을 만들어서 ContextHandle 정보주입
	FGameplayEffectSpecHandle SpecHandle = GetBaseAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(Effect, GetAbilityLevel(), ContextHandle);

	//핸들을 만들 때, SetByCaller로 Damage 지정
	if (Type != EAttributeType::DeBuffAtk)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(
		BaseGamePlayTags::Shared_SetByCaller_TickDamage,
		BaseDamage
		);
	}
	else
	{
		SpecHandle.Data->SetSetByCallerMagnitude(BaseGamePlayTags::Shared_SetByCaller_DebuffApply,BaseDamage);
		UE_LOG(LogTemp,Warning,TEXT("Debuff: %f"),BaseDamage)
	}
	

	return SpecHandle;
}



void UBaseGameplayAbility::ClearAllLooseTag(FGameplayTag Tag)
{
	while (GetBaseAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(Tag))
	{
		GetBaseAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(Tag);
	}
	
}

