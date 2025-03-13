// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerGameplayAbility_EquipBase.h"

#include "Character/PlayerCharacter.h"
#include "Components/Combat/HumanCombatComponent.h"
#include "Components/UI/PlayerUIComponent.h"

void UPlayerGameplayAbility_EquipBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPlayerGameplayAbility_EquipBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGameplayAbility_EquipBase::EquipSpell()
{
	GetHumanCombatComponentFromActorInfo()->GetPlayerCarriedSpells();
	if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
	{
		//PlayerUIComponent->OnEquipSpellChange.Broadcast(Slot,SpellID);
	}
}
