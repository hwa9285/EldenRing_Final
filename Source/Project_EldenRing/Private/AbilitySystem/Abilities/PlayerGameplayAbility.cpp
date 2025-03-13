// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"


#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Controllers/HumanController.h"


APlayerCharacter* UPlayerGameplayAbility::GetPlayerCharacterFromActorInfo() 
{
	if (!CachedPlayerCharacter.IsValid())
	{
		CachedPlayerCharacter = Cast<APlayerCharacter>(CurrentActorInfo->AvatarActor);
		if (!CachedPlayerCharacter.IsValid() && GetPlayerControllerFromActorInfo())
		{
			CachedPlayerCharacter = CachedPlayerController->GetPlayerChr();
		}
	}

	return CachedPlayerCharacter.IsValid() ? CachedPlayerCharacter.Get() : nullptr;
}

AHumanController* UPlayerGameplayAbility::GetPlayerControllerFromActorInfo() 
{
	if (!CachedPlayerController .IsValid())
	{
		CachedPlayerController = Cast<AHumanController>(CurrentActorInfo->PlayerController);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Player Controller: %s"),CachedPlayerController.IsValid()? TEXT("true") : TEXT("false"));
	return CachedPlayerController .IsValid()? CachedPlayerController .Get() : nullptr;
}

UHumanCombatComponent* UPlayerGameplayAbility::GetHumanCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetHumanCombatComponent();
}

FGameplayEffectSpecHandle UPlayerGameplayAbility::MakeEquipEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect,
	float Attack, float Cost, FGameplayTag AttackHandTag, FGameplayTag CostHandTag)
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
	AttackHandTag,
	Attack
	);
	SpecHandle.Data->SetSetByCallerMagnitude(
	CostHandTag,
	Cost
	);
	return SpecHandle;
}


FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect, float WeaponBaseDamage,float ImpulseBase, FGameplayTag AttackTypeTag)
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
	BaseGamePlayTags::Shared_SetByCaller_BaseDamage,
	WeaponBaseDamage
	);
	SpecHandle.Data->SetSetByCallerMagnitude(
		BaseGamePlayTags::Shared_SetByCaller_Impulse,
		ImpulseBase);

	if (AttackTypeTag.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(AttackTypeTag, WeaponBaseDamage);
	}
	
	
	return SpecHandle;
}

FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerDamageAndImpulseEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect,
	float BaseDamage, float ImpulseBase, float ChargedTime, FGameplayTag AttackTypeTag, int32 ComboCount)
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
	BaseGamePlayTags::Shared_SetByCaller_BaseDamage,
	BaseDamage
	);
	SpecHandle.Data->SetSetByCallerMagnitude(
		BaseGamePlayTags::Shared_SetByCaller_Impulse,
		ImpulseBase);
	SpecHandle.Data->SetSetByCallerMagnitude(
		BaseGamePlayTags::Player_SetByCaller_AttackType_Charged,
		ChargedTime);
	
	
	if (AttackTypeTag.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(AttackTypeTag, ComboCount);
	}
	
	
	return SpecHandle;
}


