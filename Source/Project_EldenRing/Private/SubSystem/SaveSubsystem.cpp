// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/SaveSubsystem.h"

#include "AbilitySystem/PlayerAttributeSet.h"
#include "Character/PlayerCharacter.h"
#include "Components/Combat/HumanCombatComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Components/Stat/StatComponent.h"
//#include "Components/Inventory/InventoryManagerComponent.h"

USaveSubsystem::USaveSubsystem()
{
	FConsumeInfo InitItem;
	InitItem.ConsumeID = NAME_None;
	InitItem.Quantity = 0;
	InitItem.Icon = nullptr;
	InitItem.ConsumeType = EConsumeType::Horse;
	
	SaveCombatData.SaveCarriedItem.Init(InitItem,3);
	SaveCombatData.SaverCarriedSpell.Init(NAME_None,6);
	SaveCombatData.SaveEquipItemIndex = 0;
	SaveCombatData.SaveEquipSpellIndex = 0;
	//SaveCombatData.SaveCarriedPrimaryWeaponMap = SaveCombatComponent->GetCarriedPrimaryWeaponMap();
	//SaveCombatData.SaveCarriedSecondaryWeaponMap = SaveCombatComponent->GetCarriedSecondaryWeaponMap();
	SaveCombatData.SaveEquipLeftWeaponIndex = 0;
	SaveCombatData.SaveEquipRightWeaponIndex = 0;

	SaveAttributeData.MaxHP = 100.f;
	SaveAttributeData.MaxFP = 70.f;
	SaveAttributeData.MaxStamina = 100.f;
	SaveAttributeData.BaseAttack = 15.f;

	
	SaveStatData.SaveBaseStat.Add(EStatsType::Strength, 10);
	SaveStatData.SaveBaseStat.Add(EStatsType::Intellect, 5);
	SaveStatData.SaveBaseStat.Add(EStatsType::Agility, 10);
	SaveStatData.SaveBaseStat.Add(EStatsType::Endurance, 10);
	SaveStatData.SaveBaseStat.Add(EStatsType::Faith, 10);
	SaveStatData.SaveBaseStat.Add(EStatsType::Mentality, 10);
	SaveStatData.SaveBaseStat.Add(EStatsType::Mystery, 10);
	SaveStatData.SaveBaseStat.Add(EStatsType::Vitality, 10);
	SaveStatData.SaveBaseStat.Add(EStatsType::Level, 1);
	SaveStatData.SaveCurrentLevel = 1;


	//SaveInventoryData.SaveInventoryMap = 
	
}

void USaveSubsystem::SaveData(APlayerCharacter* PlayerCharacter)
{
	SaveCombatComponent(PlayerCharacter->GetHumanCombatComponent());
	SaveAttributeSet(PlayerCharacter->GetBaseAttributeSet());
	RespawnTransform = PlayerCharacter->GetActorTransform();
	SaveStatComponent(PlayerCharacter->GetPlayerStatComponent());
	SaveInventoryComponent(PlayerCharacter->GetInventoryManager());
}

void USaveSubsystem::SaveDataByPlayerDeath(APlayerCharacter* PlayerCharacter)
{
	SaveCombatComponent(PlayerCharacter->GetHumanCombatComponent());
	SaveAttributeSet(PlayerCharacter->GetBaseAttributeSet());
	SaveStatComponent(PlayerCharacter->GetPlayerStatComponent());
	SaveInventoryComponent(PlayerCharacter->GetInventoryManager());
}

void USaveSubsystem::SaveCombatComponent(UHumanCombatComponent* SaveCombatComponent)
{
	SaveCombatData.SaveCarriedItem = SaveCombatComponent->GetCarriedItem();
	SaveCombatData.SaverCarriedSpell =SaveCombatComponent->GetPlayerCarriedSpells();
	SaveCombatData.SaveEquipItemIndex = SaveCombatComponent->CurrentEquipItemIndex;
	SaveCombatData.SaveEquipSpellIndex = SaveCombatComponent->CurrentEquipSpellIndex;
	SaveCombatData.SaveCarriedPrimaryWeaponMap = SaveCombatComponent->GetCarriedPrimaryWeaponMap();
	SaveCombatData.SaveCarriedSecondaryWeaponMap = SaveCombatComponent->GetCarriedSecondaryWeaponMap();
	SaveCombatData.SaveEquipLeftWeaponIndex = SaveCombatComponent->CurrentEquipLeftWeaponIndex;
	SaveCombatData.SaveEquipRightWeaponIndex = SaveCombatComponent->CurrentEquipRightWeaponIndex;
	
}

void USaveSubsystem::SaveAttributeSet(UPlayerAttributeSet* SaveAttributeSet)
{
	SaveAttributeData.MaxHP = SaveAttributeSet->GetMaxHp();
	SaveAttributeData.MaxFP = SaveAttributeSet->GetMaxFP();
	SaveAttributeData.MaxStamina = SaveAttributeSet->GetMaxStamina();
	SaveAttributeData.BaseAttack = SaveAttributeSet->GetBaseAttack();
}

void USaveSubsystem::SaveStatComponent(UStatComponent* SaveStatComponent)
{
	SaveStatData.SaveBaseStat = SaveStatComponent->GetBaseStats();
	SaveStatData.SaveCurrentLevel = SaveStatComponent->GetCurrentLevel();
}

void USaveSubsystem::SaveInventoryComponent(UInventoryManagerComponent* SaveInventoryComponent)
{
	SaveInventoryData.SaveInventoryMap = SaveInventoryComponent->InventoryMap;
	SaveInventoryData.SaveSpellEquipInventorySlots = SaveInventoryComponent->GetSpellEquipInventorySlots();
	SaveInventoryData.SaveConsumeEquipSlots = SaveInventoryComponent->ConsumeEquipSlots;
}