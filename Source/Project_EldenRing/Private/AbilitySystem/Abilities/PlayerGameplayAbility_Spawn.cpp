// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerGameplayAbility_Spawn.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/Combat/HumanCombatComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Item/Equipment/Weapon/PlayerWeaponBase.h"
#include "Item/Subsystem/InventoryGameInstanceSubsystem.h"


void UPlayerGameplayAbility_Spawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ChangedWeapon();
}

void UPlayerGameplayAbility_Spawn::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UPlayerGameplayAbility_Spawn::ChangedWeapon()
{
	//새로 생성할 무기
	Item_ID = GetPlayerCharacterFromActorInfo()->GetInventoryManager()->ItemID.ToString();
	Reinforce_ID = GetPlayerCharacterFromActorInfo()->GetInventoryManager()->ReinforceID.ToString();
	RowName = Item_ID + "." + Reinforce_ID;

	CurrentType = GetPlayerCharacterFromActorInfo()->GetInventoryManager()->Type;
	
	UInventoryGameInstanceSubsystem* Sub = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();

	switch (CurrentType)
	{
	case ESpawnWeaponType::Left:
		SelectedInventory = GetHumanCombatComponentFromActorInfo()->GetPlayerCarriedSecondaryWeaponMap();
		WeaponData = Sub->GetWeaponLeftRowData(FName(*Item_ID));
		HandSocketName = "hand_lSocket";
		break;
	case ESpawnWeaponType::Right:
		SelectedInventory = GetHumanCombatComponentFromActorInfo()->GetPlayerCarriedPrimaryWeaponMap();
		WeaponData = Sub->GetWeaponRightRowData(FName(*Item_ID));
		HandSocketName = "hand_rSocket";
		break;
	}
	Slot = GetPlayerCharacterFromActorInfo()->GetInventoryManager()->TestIndex;

	CurrentEquipIndex = GetPlayerCharacterFromActorInfo()->GetEquipIndexFromType(CurrentType);
	
	//이미 값이 있는지 확인
	if (SelectedInventory.Contains(Slot))
	{
		//인덱스를 통해 현재 착용중인 무기이면 플레이어에서 착용중인 무기를 unequip하고 새로운 무기 생성한다음 부착시키고 컴뱃에 넣고 착용까지해야함
		if (Slot == CurrentEquipIndex)
		{
			//삭제할 무기
			//이미 착용중이면 1. Unequip 2. destroy 3.Spawn  4.착용
			GetPlayerCharacterFromActorInfo()->UnequipHandle(CurrentType,SelectedInventory[Slot]);
			SelectedInventory[Slot]->Destroy();

			bIsEquipped = true;
		}
		else
		{
			//착용중은 아니면 1. 기존 무기 UnEquip 2. DesTroy
			GetPlayerCharacterFromActorInfo()->UnequipHandle(CurrentType,SelectedInventory[Slot]);
			SelectedInventory[Slot]->Destroy();
		}
	}
	//값이 없다면 그냥 Add하고 Spawn
	SpawnNewWeapon();
}

void UPlayerGameplayAbility_Spawn::SpawnNewWeapon()
{
	//스폰 파라미터 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetAvatarActorFromActorInfo();
	SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	
	//스폰
	APlayerWeaponBase* SpawnWeapon = GetWorld()->SpawnActor<APlayerWeaponBase>(WeaponData.WeaponBP, SpawnParams);
	
	if (SpawnWeapon)
	{
		SpawnWeapon->SetReinforceID(FName(*Reinforce_ID));
		//부착
		SpawnWeapon->AttachToComponent(GetOwningComponentFromActorInfo(),AttachmentRules,SocketName);
		
		//히든처리
		SpawnWeapon->SetActorHiddenInGame(true);
		
		//컴뱃컴포넌트에 등록
		GetHumanCombatComponentFromActorInfo()->RegisterPlayerWeaponSpawn(Slot,
			CurrentType,
			SpawnWeapon,
			false);

		if (bIsEquipped)
		{
			GetPlayerCharacterFromActorInfo()->EquipHandle(CurrentType,SpawnWeapon);
			SpawnWeapon->AttachToComponent(GetOwningComponentFromActorInfo(),AttachmentRules,HandSocketName);
			SpawnWeapon->SetActorHiddenInGame(false);
			bIsEquipped = false;
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("SpawnWeapons could not be found"));
	}
}
