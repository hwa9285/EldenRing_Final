// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerGameplayAbility_Destroy.h"

#include "Character/PlayerCharacter.h"
#include "Components/Combat/HumanCombatComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Item/Equipment/Weapon/PlayerWeaponBase.h"
#include "Item/Subsystem/InventoryGameInstanceSubsystem.h"

void UPlayerGameplayAbility_Destroy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	DestroyWeapon();
}

void UPlayerGameplayAbility_Destroy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGameplayAbility_Destroy::DestroyWeapon()
{
	CurrentType = GetPlayerCharacterFromActorInfo()->GetInventoryManager()->Type;
	
	//선택한 스롯 인덱스
	Slot = GetPlayerCharacterFromActorInfo()->GetInventoryManager()->TestIndex;
	
	GetHumanCombatComponentFromActorInfo()->RemovePlayerWeapon(Slot,CurrentType);
	//GetPlayerCharacterFromActorInfo()->SetEquipIndexFromType(CurrentType,-1);
	
}
