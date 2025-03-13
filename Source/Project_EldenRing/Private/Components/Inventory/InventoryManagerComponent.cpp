// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryManagerComponent.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Item/Equipment/EquipmentBase.h"
#include "Character/PlayerCharacter.h"
#include "Components/Widget.h"
#include "Components/Combat/HumanCombatComponent.h"
#include "Item/Subsystem/InventoryGameInstanceSubsystem.h"
#include "Components/Inventory/InventotySystemComponent.h"
#include "Components/UI/PlayerUIComponent.h"


UInventoryManagerComponent::UInventoryManagerComponent()
{

}

void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	EmptyItemInfos.ItemID = "F";
	EmptyItemInfos.ItemTypeID = "F";
	EmptyItemInfos.Quantity = 1;
	
	// UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	// Subsystem->IsGameStart;
	// if (Subsystem->IsGameStart)
	// {
	// 	startUpInventerManager();
	// 	Subsystem->IsGameStart = false;
	// }
}




// void UInventoryManagerComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (OtherActor)
// 	{
// 		// 현재 아이템 저장
// 		CurrentInteractItem = OtherActor;
// 		
// 		// 1. 인터페이스 있는지
// 		if (OtherActor->Implements<UItemPickupInterface>())
// 		{
// 			ItemName = IItemPickupInterface::Execute_GetName(OtherActor);
// 			ItemInfo = IItemPickupInterface::Execute_GetItemInfos(OtherActor);
// 			if (ItemName.IsValid())
// 			{
// 				//UE_LOG(LogTemp, Error, TEXT("FindRow returned %s"), *ItemName.ToString());
// 			}
// 			else
// 			{
// 				UE_LOG(LogTemp, Error, TEXT("FindRow returned NULL"));
// 				return;
// 			}
// 			if (!CurrentInteractItem)
// 			{
// 				UE_LOG(LogTemp, Error, TEXT("Item not Found"));
// 				return;
// 			}
// 			UE_LOG(LogTemp, Error, TEXT("ItemName returned %s"), *ItemName.ToString());
//
// 			if (ItemName == "Talk")
// 			{
// 				SelectInteractType = EInteractType::Talk;
// 			}
// 			else if (ItemName == "SavePoint")
// 			{
// 				SelectInteractType = EInteractType::SavePoint;
// 			}
// 			else
// 			{
// 				SelectInteractType = EInteractType::PickUpItem;
// 			}
// 		}
// 		else 
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("OtherActor does not implement UItemPickupInterface."));
// 		}
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("OtherActor is null."));
// 	}
// }



void UInventoryManagerComponent::AddToInventory(AActor* InteractItem)
{
	CurrentInteractItem = InteractItem;

	ItemName = IItemPickupInterface::Execute_GetName(CurrentInteractItem);
	ItemInfo = IItemPickupInterface::Execute_GetItemInfos(CurrentInteractItem);
	if (ItemName.IsValid())
	{
		//UE_LOG(LogTemp, Error, TEXT("FindRow returned %s"), *ItemName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FindRow returned NULL"));
		return;
	}
	if (!CurrentInteractItem)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentInteractItem is nullptr!"));
		return;
	}
	UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	EBaseInventoryType InventoryType = Subsystem->GetDroppedItemData(ItemInfo.ItemID).InventoryType;
    FName NewItemID = Subsystem->GetDroppedItemData(ItemInfo.ItemID).ItemName;
	UPaperSprite* Icon = Subsystem->GetDroppedItemData(ItemInfo.ItemID).ItemIcon;

	
	CheckQuantity(ItemInfo, InventoryType);

	OnAddtoInventory.Broadcast();


	if (!CachedUIInterface.IsValid())
	{
		CachedUIInterface = TWeakInterfacePtr<IPawnUIInterface>(GetOwner());
	}
	//
	check(CachedUIInterface.IsValid());

	if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
	{
		PlayerUIComponent->	OnAddNewItem.Broadcast(NewItemID, Icon);
	}
	

	CurrentInteractItem->Destroy();
	CurrentInteractItem = nullptr;
	// SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UInventoryManagerComponent::AddToInventoryWithItemInfo(FItemInfos _ItemInfos)
{
	UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	EBaseInventoryType InventoryType = Subsystem->GetDroppedItemData(_ItemInfos.ItemID).InventoryType;
	
	//UE_LOG(LogTemp, Warning, TEXT("Bind."));
	OnAddtoInventory.Broadcast();
	
	CheckQuantity(_ItemInfos, InventoryType);
}


void UInventoryManagerComponent::BroadcastToUpdateWidget(int32 Index, FItemInfos _ItemInfos,bool Equiped, EBaseInventoryType EquipType)
{
	ChangedItemDelegate.Broadcast(Index, _ItemInfos, Equiped, EquipType);
}

void UInventoryManagerComponent::BroadCastHoverEvent(FItemInfos _ItemInfos, EBaseInventoryType _EquipType)
{
	OnHoverEventDelegate.Broadcast(_ItemInfos, _EquipType);
}


void UInventoryManagerComponent::AddSpellToInventory(uint8 SelectedSlotIndex, FName SelectedSpellID)
{
	UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	FDroppedItemInfo SpellData = Subsystem->GetDroppedItemData(SelectedSpellID);

	//선택한 슬롯과는 관계 없이, 비어있는 슬롯 중 가장 낮은 슬롯에 마법을 장착해야하므로 for문 수행
	for (uint8 SlotIndex = 0; SlotIndex < SpellEquipInventorySlots.Num(); SlotIndex++)
	{
		//비어있는 슬롯을 발견한 경우
		if (SpellEquipInventorySlots[SlotIndex] == NAME_None)
		{
			SpellEquipInventorySlots[SlotIndex] = SelectedSpellID;
			GetPlayerCharacter()->GetHumanCombatComponent()->RegisterCarriedSpell(SlotIndex,SelectedSpellID);
			return;
		}
	}

	//모두 장착중일경우 선택한 슬롯에 장착(교체)
	SpellEquipInventorySlots[SelectedSlotIndex] = SelectedSpellID;
	GetPlayerCharacter()->GetHumanCombatComponent()->RegisterCarriedSpell(SelectedSlotIndex,SelectedSpellID);
}


void UInventoryManagerComponent::AddToInventoryByNPC(FItemInfos _ItemInfos)
{
	UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	EBaseInventoryType InventoryType = Subsystem->GetDroppedItemData(_ItemInfos.ItemID).InventoryType;
	
	FName NewItemID = Subsystem->GetDroppedItemData(_ItemInfos.ItemID).ItemName;
	UPaperSprite* Icon = Subsystem->GetDroppedItemData(_ItemInfos.ItemID).ItemIcon;
	
	//UE_LOG(LogTemp, Warning, TEXT("Bind."));
	OnAddtoInventory.Broadcast();

	if (!CachedUIInterface.IsValid())
	{
		CachedUIInterface = TWeakInterfacePtr<IPawnUIInterface>(GetOwner());
	}
	//
	check(CachedUIInterface.IsValid());

	if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
	{
		PlayerUIComponent->	OnAddNewItemByNPC.Broadcast(NewItemID, Icon);
	}
	
	CheckQuantity(_ItemInfos, InventoryType);
}

APlayerCharacter* UInventoryManagerComponent::GetPlayerCharacter()
{
	return Cast<APlayerCharacter>(GetOwner());
}

void UInventoryManagerComponent::PopulateUniformGrid(UUniformGridPanel* InUniformGridPanel, UUserWidget* NewWidget)
{
	if (!InUniformGridPanel || !NewWidget)
	{
		return;
	}
}

void UInventoryManagerComponent::CheckQuantity(FItemInfos _ItemInfos, EBaseInventoryType InventoryType)
{
	if (InventoryType == EBaseInventoryType::Potion || InventoryType == EBaseInventoryType::Material || InventoryType == EBaseInventoryType::MakeItem)
	{
		//TArray<FItemInfos>& ItemInfoArray = InventoryMap[InventoryType].TypeItems;
		bool bFound = false;
		int32 Index = 0;

	
		for (int32 i = InventoryMap[InventoryType].TypeItemsPtr.Num() - 1; i >= 0; i--)
		{
			if (InventoryMap[InventoryType].TypeItemsPtr[i]->ItemInfo.ItemID == _ItemInfos.ItemID)
			{
					Index = i;
					bFound = true;
					break;
			}
		}

		

		// // 이미 해당 ItemID가 있는지 검사 이미 같은 이름을 가지고 있는지
		// for (int i = 0; i < ItemInfoArray.Num(); i++)
		// {
		// 	if (ItemInfoArray[i].ItemID == _ItemInfos.ItemID)
		// 	{
		// 		Index = i;
		// 		bFound = true;
		// 		break;
		// 	}
		// }
		if (bFound)
		{

			//UE_LOG(LogTemp, Warning, TEXT("Find AllItems"));
			
			InventoryMap[InventoryType].TypeItemsPtr[Index]->ItemInfo.Quantity = InventoryMap[InventoryType].TypeItemsPtr[Index]->ItemInfo.Quantity + _ItemInfos.Quantity;
			BroadcastToUpdateWidget(Index,InventoryMap[InventoryType].TypeItemsPtr[Index]->ItemInfo, false, InventoryType);
			//_ItemInfos.Quantity = InventoryMap[InventoryType].TypeItemsPtr[Index]->ItemInfo.Quantity;
			// for (int32 i = InventoryMap[InventoryType].TypeItemsPtr.Num() - 1; i >= 0; i--)
			// {
			// 	if (InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[i]->ItemInfo.ItemID == _ItemInfos.ItemID)
			// 	{
			// 		BroadcastToUpdateWidget(i,_ItemInfos, false, EBaseInventoryType::Item);
			// 		
			// 		
			// 		break;
			// 	}
			// }
			int32 FoundIndex = InventoryMap[EBaseInventoryType::Item].TypeItemsPtr.Find(InventoryMap[InventoryType].TypeItemsPtr[Index]);
			BroadcastToUpdateWidget(FoundIndex,InventoryMap[InventoryType].TypeItemsPtr[Index]->ItemInfo, false, EBaseInventoryType::Item);
		}
		else
		{
			UItemInfo* SharedItem = NewObject<UItemInfo>();
			SharedItem->ItemInfo = _ItemInfos;
			InventoryMap[EBaseInventoryType::Item].TypeItemsPtr.Add(SharedItem);
			InventoryMap[InventoryType].TypeItemsPtr.Add(SharedItem);
			BroadcastToUpdateWidget(InventoryMap[EBaseInventoryType::Item].TypeItemsPtr.Num() - 1,_ItemInfos, false, EBaseInventoryType::Item);
			BroadcastToUpdateWidget(InventoryMap[InventoryType].TypeItemsPtr.Num() - 1, _ItemInfos, false, InventoryType);
			// InventoryMap[EBaseInventoryType::Item].TypeItems.Add(_ItemInfos);
			// BroadcastToUpdateWidget(InventoryMap[EBaseInventoryType::Item].TypeItems.Num() - 1,_ItemInfos, false, EBaseInventoryType::Item);
			// InventoryMap[InventoryType].TypeItems.Add(_ItemInfos);
			// BroadcastToUpdateWidget(InventoryMap[InventoryType].TypeItems.Num() - 1, _ItemInfos, false, InventoryType);
			//UE_LOG(LogTemp, Warning, TEXT("Item not exists."));
		}
	}
	else
	{
		UItemInfo* SharedItem = NewObject<UItemInfo>();
		SharedItem->ItemInfo = _ItemInfos;
		InventoryMap[EBaseInventoryType::Item].TypeItemsPtr.Add(SharedItem);
		InventoryMap[InventoryType].TypeItemsPtr.Add(SharedItem);
		BroadcastToUpdateWidget(InventoryMap[EBaseInventoryType::Item].TypeItemsPtr.Num() - 1,_ItemInfos, false, EBaseInventoryType::Item);
		BroadcastToUpdateWidget(InventoryMap[InventoryType].TypeItemsPtr.Num() - 1, _ItemInfos, false, InventoryType);
		
		// InventoryMap[EBaseInventoryType::Item].TypeItems.Add(_ItemInfos);
		// BroadcastToUpdateWidget(InventoryMap[EBaseInventoryType::Item].TypeItems.Num() - 1, _ItemInfos, false, EBaseInventoryType::Item);
		// InventoryMap[InventoryType].TypeItems.Add(_ItemInfos);
		// BroadcastToUpdateWidget(InventoryMap[InventoryType].TypeItems.Num() - 1, _ItemInfos, false, InventoryType);
	}
}

void UInventoryManagerComponent::ActiveCollision()
{
	//SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//OnOffCollisioinTimerHandle.
}

void UInventoryManagerComponent::AddItemInfoPtr(EBaseInventoryType InventoryType, UItemInfo* SharedItem)
{
	InventoryMap[InventoryType].TypeItemsPtr.Emplace(SharedItem);
	BroadcastToUpdateWidget(InventoryMap[InventoryType].TypeItemsPtr.Num()-1, SharedItem->ItemInfo, true, InventoryType);
}

void UInventoryManagerComponent::AddItemInfoPtrIndex(EBaseInventoryType InventoryType, UItemInfo* SharedItem,
	int32 Index)
{
	InventoryMap[InventoryType].TypeItemsPtr.EmplaceAt(Index, SharedItem);
	BroadcastToUpdateWidget(Index, SharedItem->ItemInfo, true, InventoryType);
}


void UInventoryManagerComponent::DropItem(int32 ItemIndex, TSubclassOf<ADroppedItemBase> DroppedItemClass)
{
	//SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	FVector SpawnLocation = GetPlayerCharacter()->GetActorLocation() + (GetPlayerCharacter()->GetActorForwardVector()*(100));
	SpawnLocation.Z += 50.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetPlayerCharacter()); 

	// 바닥을 찾기 위해 아래 방향으로 라인트레이스
	FVector TraceStart = SpawnLocation;
	FVector TraceEnd = TraceStart - FVector(0.f, 0.f, 200.f);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	
	if (bHit)
	{
		SpawnLocation = HitResult.Location; 
	}

	FRotator SpawnRotation;
	if (DroppedItemClass)
	{
		ADroppedItemBase* SpawnedItem = GetWorld()->SpawnActorDeferred<ADroppedItemBase>(DroppedItemClass, FTransform(SpawnRotation, SpawnLocation), GetPlayerCharacter(), nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		// SpawnedItem->DroppedItemName = InventoryMap[EBaseInventoryType::Item].TypeItems[ItemIndex].ItemID;
		// SpawnedItem->WeaponStatisticsName = InventoryMap[EBaseInventoryType::Item].TypeItems[ItemIndex].ItemTypeID;
		// SpawnedItem->DroppedItemAmount = InventoryMap[EBaseInventoryType::Item].TypeItems[ItemIndex].Quantity;
		SpawnedItem->DroppedItemName = InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[ItemIndex]->ItemInfo.ItemID;
		SpawnedItem->WeaponStatisticsName = InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[ItemIndex]->ItemInfo.ItemTypeID;
		SpawnedItem->DroppedItemAmount = InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[ItemIndex]->ItemInfo.Quantity;
		SpawnedItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
		
		UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
		FDroppedItemInfo DroppedItemData = Subsystem->GetDroppedItemData(SpawnedItem->DroppedItemName);
		


 
		UItemInfo* SharedItem;
		SharedItem = InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[ItemIndex];

		for (int32 i = InventoryMap[DroppedItemData.InventoryType].TypeItemsPtr.Num() - 1; i >= 0; i--) 
		{
			if (InventoryMap[DroppedItemData.InventoryType].TypeItemsPtr[i] == SharedItem) 
			{
				InventoryMap[DroppedItemData.InventoryType].TypeItemsPtr.RemoveAt(i);
				BroadcastToUpdateWidget(i, EmptyItemInfos, false, DroppedItemData.InventoryType);
				break;
			}
		}
		
		// for (int i = 0; i < InventoryMap[DroppedItemData.InventoryType].TypeItemsPtr.Num(); i++)
		// {
		// 	if (InventoryMap[DroppedItemData.InventoryType].TypeItems[i].ItemID == SpawnedItem->DroppedItemName && InventoryMap[DroppedItemData.InventoryType].TypeItems[i].ItemTypeID == SpawnedItem->WeaponStatisticsName)
		// 	{
		//
		// 		InventoryMap[DroppedItemData.InventoryType].TypeItems.RemoveAt(i);
		// 		InventoryMap[DroppedItemData.InventoryType].TypeItemsPtr.RemoveAt(i);
		// 		BroadcastToUpdateWidget(i, EmptyItemInfos, false, DroppedItemData.InventoryType);
		// 		break;
		// 	}
		// }
	}
	
	GetWorld()->GetTimerManager().SetTimer(OnOffCollisioinTimerHandle, this, &UInventoryManagerComponent::ActiveCollision, 2.0f, false);
	
	//InventoryMap[EBaseInventoryType::Item].TypeItems.RemoveAt(ItemIndex);
	InventoryMap[EBaseInventoryType::Item].TypeItemsPtr.RemoveAt(ItemIndex);
	BroadcastToUpdateWidget(ItemIndex, EmptyItemInfos, false, EBaseInventoryType::Item);
}


void UInventoryManagerComponent::UpdateAllSlot(TArray<UWidget*> Widgets, EBaseInventoryType _EquipType)
{
	if (Widgets.Num() > 0)
	{
		for (int i = 0; i < Widgets.Num(); i++)
		{
			if (InventoryMap[_EquipType].TypeItemsPtr.IsValidIndex(i))
			{
				BroadcastToUpdateWidget(i, InventoryMap[_EquipType].TypeItemsPtr[i]->ItemInfo, false, _EquipType);
			}
			else
			{
				BroadcastToUpdateWidget(i, EmptyItemInfos, false, _EquipType);
			}
		}
	}
}

void UInventoryManagerComponent::UpdateAllMaxInt(int32 MaxInt, EBaseInventoryType _EquipType)
{
	// for (int i = 0; i < MaxInt; i++)
	// {
	// 	if (InventoryMap[_EquipType].TypeItemsPtr[i] != nullptr)
	// 	{
	// 		if (InventoryMap[_EquipType].TypeItemsPtr.IsValidIndex(i))
	// 		{
	// 			BroadcastToUpdateWidget(i, InventoryMap[_EquipType].TypeItemsPtr[i]->ItemInfo, false, _EquipType);
	// 		}
	// 		else
	// 		{
	// 			BroadcastToUpdateWidget(i, EmptyItemInfos, false, _EquipType);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		BroadcastToUpdateWidget(i, EmptyItemInfos, false, _EquipType);
	// 	}
	// }
	for (int i = 0; i < MaxInt; i++)
	{
		// 1) 먼저 인덱스가 유효한지 확인
		if (InventoryMap[_EquipType].TypeItemsPtr.IsValidIndex(i))
		{
			// 2) 배열에 접근 가능해졌으므로, nullptr인지 검사
			if (InventoryMap[_EquipType].TypeItemsPtr[i] != nullptr)
			{
				BroadcastToUpdateWidget(i, InventoryMap[_EquipType].TypeItemsPtr[i]->ItemInfo, false, _EquipType);
			}
			else
			{
				// 유효한 인덱스지만 아이템 포인터가 nullptr
				BroadcastToUpdateWidget(i, EmptyItemInfos, false, _EquipType);
			}
		}
		else
		{
			// 유효하지 않은 인덱스이므로 빈 슬롯으로 처리
			BroadcastToUpdateWidget(i, EmptyItemInfos, false, _EquipType);
		}
	}
	
}

void UInventoryManagerComponent::UpdateAllItem()
{
	UpdateQuantityInventory("100", MaxHealthPotionInt);
	UpdateQuantityInventory("101", MaxHealthPotionInt);

	FConsumeInfo ConsumeInfo;
	UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	
	for (int32 i = 0; i < ConsumeEquipSlots.Num(); i++)
	{
		ConsumeInfo = ConsumeEquipSlots[i];
		GetPlayerCharacter()->GetHumanCombatComponent()->RegisterItem(i, ConsumeInfo);
		UE_LOG(LogTemp, Warning, TEXT("Consume ID: %s"), *ConsumeInfo.ConsumeID.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Consume ID: %i"), ConsumeInfo.Quantity);
	}
	
	for (auto& KeyValue : InventoryMap)
	{
		EBaseInventoryType KeyType = KeyValue.Key;
		
		if (KeyType == EBaseInventoryType::Item)
		{
			UpdateAllMaxInt(40, KeyType);
		}
		else
		{
			UpdateAllMaxInt(10, KeyType);
		}
		
	}

	UpdateAllequipedWeapon();

}

bool UInventoryManagerComponent::FindItemAndSet(int32 _FindIndex, EBaseInventoryType _FindEquipType, EBaseInventoryType& _ReturnEquipType, int32& _ReturnIndex)
{
	// 먼저 InventoryMap에 해당 키가 존재하는지 체크
	if (!InventoryMap.Contains(_FindEquipType))
	{
		return false;
	}
	
	if (!InventoryMap.Contains(_FindEquipType) || _FindIndex < 0 || _FindIndex >= InventoryMap[_FindEquipType].TypeItemsPtr.Num())
	{
		return false;
	}
	
	if (!InventoryMap[_FindEquipType].TypeItemsPtr[_FindIndex])
	{
		return false;
	}
	
	int PrimaryWeaponIndex = InventoryMap[EBaseInventoryType::PrimaryWeapon].TypeItemsPtr.Find(InventoryMap[_FindEquipType].TypeItemsPtr[_FindIndex]);
	if (PrimaryWeaponIndex != -1)
	{
		_ReturnEquipType = EBaseInventoryType::PrimaryWeapon;
		_ReturnIndex = PrimaryWeaponIndex;
		return true;
	}

	int SecondaryWeaponIndex = InventoryMap[EBaseInventoryType::SecondaryWeapon].TypeItemsPtr.Find(InventoryMap[_FindEquipType].TypeItemsPtr[_FindIndex]);
	if (SecondaryWeaponIndex != -1)
	{
		_ReturnEquipType = EBaseInventoryType::SecondaryWeapon;
		_ReturnIndex = SecondaryWeaponIndex;
		return true;
	}
	
	// 둘 다 실패
	return false;
}

void UInventoryManagerComponent::SwapIfChoiceIsValidInLeftLight(EBaseInventoryType _ReturnEquipType, int32 _ReturnIndex)
{
	auto Temp = InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex];
	InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex] = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex];
	InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex] = Temp;

	BroadcastToUpdateWidget(_ReturnIndex, InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex]->ItemInfo, true, _ReturnEquipType);
	BroadcastToUpdateWidget(SelectIndex, InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo, true, SelectInventoryType);
	FGameplayTag RemoveTag = BaseGamePlayTags::Player_Event_RemoveWeapon;
	FGameplayTag AddTag = BaseGamePlayTags::Player_Event_AddWeapon;

	if (_ReturnEquipType == EBaseInventoryType::PrimaryWeapon)
	{
		Type = ESpawnWeaponType::Right;
	}
	else
	{
		Type = ESpawnWeaponType::Left;
	}
	TestIndex = _ReturnIndex;
	ItemID = InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex]->ItemInfo.ItemID;
	ReinforceID = InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex]->ItemInfo.ItemTypeID;
	//GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(RemoveTag);
    GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(AddTag);


	if (SelectInventoryType == EBaseInventoryType::PrimaryWeapon)
	{
		Type = ESpawnWeaponType::Right;
	}
	else
	{
		Type = ESpawnWeaponType::Left;
	}
	TestIndex = SelectIndex;;
	ItemID = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo.ItemID;
	ReinforceID = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo.ItemTypeID;
	//GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(RemoveTag);
	GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(AddTag);
}

void UInventoryManagerComponent::SwapEmptySlotToHasSlot(EBaseInventoryType _ReturnEquipType, int32 _ReturnIndex)
{
	//값 미리 복사 -> 지우는 태그 보내고 -> 나도 널값 추가
	InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex] = InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex];

	
	FGameplayTag RemoveTag = BaseGamePlayTags::Player_Event_RemoveWeapon;
	FGameplayTag AddTag = BaseGamePlayTags::Player_Event_AddWeapon;


	if (_ReturnEquipType == EBaseInventoryType::PrimaryWeapon)
	{
		Type = ESpawnWeaponType::Right;
	}
	else
	{
		Type = ESpawnWeaponType::Left;
	}
	TestIndex = _ReturnIndex;
	ItemID = InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex]->ItemInfo.ItemID;
	ReinforceID = InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex]->ItemInfo.ItemTypeID;
	GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(RemoveTag);
	InventoryMap[_ReturnEquipType].TypeItemsPtr[_ReturnIndex] = nullptr;
	

	
	if (SelectInventoryType == EBaseInventoryType::PrimaryWeapon)
	{
		Type = ESpawnWeaponType::Right;
	}
	else
	{
		Type = ESpawnWeaponType::Left;
	}
	TestIndex = SelectIndex;;
	ItemID = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo.ItemID;
	ReinforceID = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo.ItemTypeID;
	//GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(RemoveTag);
	GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(AddTag);
	
	FItemInfos Test;
	Test.ItemID = "F";
	Test.ItemTypeID = "F";
	Test.Quantity = 1;
	
	BroadcastToUpdateWidget(_ReturnIndex, Test, false, _ReturnEquipType);
	BroadcastToUpdateWidget(SelectIndex, InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo, true, SelectInventoryType);
}

void UInventoryManagerComponent::SwapFindSlotToHasSlot(EBaseInventoryType _FindEquipType, int32 _FindIndex)
{
	FGameplayTag RemoveTag = BaseGamePlayTags::Player_Event_RemoveWeapon;
	FGameplayTag AddTag = BaseGamePlayTags::Player_Event_AddWeapon;

	if (SelectInventoryType == EBaseInventoryType::PrimaryWeapon)
	{
		Type = ESpawnWeaponType::Right;
	}
	else
	{
		Type = ESpawnWeaponType::Left;
	}
	TestIndex = SelectIndex;;
	ItemID = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo.ItemID;
	ReinforceID = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo.ItemTypeID;
	//GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(RemoveTag);

	UItemInfo* SharedItem = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex];
	InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex] = InventoryMap[_FindEquipType].TypeItemsPtr[_FindIndex];


	if (SelectInventoryType == EBaseInventoryType::PrimaryWeapon)
	{
		Type = ESpawnWeaponType::Right;
	}
	else
	{
		Type = ESpawnWeaponType::Left;
	}
	TestIndex = SelectIndex;;
	ItemID = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo.ItemID;
	ReinforceID = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo.ItemTypeID;
	GetPlayerCharacter()->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(AddTag);


	//UItemInfo* SharedItem = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex];
	int32 FoundIndex = InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.Find(SharedItem);
	if (FoundIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Found FoundIndex"));
		return;
	}
	InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr[FoundIndex] = InventoryMap[_FindEquipType].TypeItemsPtr[_FindIndex];


	BroadcastToUpdateWidget(_FindIndex, InventoryMap[_FindEquipType].TypeItemsPtr[_FindIndex]->ItemInfo , true, _FindEquipType);
	BroadcastToUpdateWidget(SelectIndex, InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex]->ItemInfo , false, SelectInventoryType);
	UpdateAllWeaponInfo(SharedItem);
	UpdateAllWeaponInfo(InventoryMap[_FindEquipType].TypeItemsPtr[_FindIndex]);
}

bool UInventoryManagerComponent::IsValidGetItemInfo(EBaseInventoryType InventoryType, int32 Index,
                                                    bool Equiped, FItemInfos& _ItemInfos ,UItemInfo*& SharedItem)
{
	if (InventoryMap[InventoryType].TypeItemsPtr.IsValidIndex(Index))
	{
		_ItemInfos = InventoryMap[InventoryType].TypeItemsPtr[Index]->ItemInfo;
		SharedItem = InventoryMap[InventoryType].TypeItemsPtr[Index];
		BroadcastToUpdateWidget(Index, _ItemInfos, Equiped, InventoryType);
	}
	else
	{
		_ItemInfos.Quantity = 1;
		_ItemInfos.ItemID = "F";
		_ItemInfos.ItemTypeID = "F";
		SharedItem = nullptr;
		BroadcastToUpdateWidget(Index, EmptyItemInfos, false, InventoryType);
	}
	return InventoryMap[InventoryType].TypeItemsPtr.IsValidIndex(Index);
}

void UInventoryManagerComponent::RemoveItemAllequipedWeaponInfoPtr(EBaseInventoryType InventoryType, int32 Index)
{
	if (InventoryMap[InventoryType].TypeItemsPtr.IsValidIndex(Index))
	{
		InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.Remove(InventoryMap[InventoryType].TypeItemsPtr[Index]);
		FItemInfos _ItemInfos;
		UItemInfo* SharedItem;
		for (int i = 0; i < InventoryMap[InventoryType].TypeItemsPtr.Num(); i++)
		{
			IsValidGetItemInfo(EBaseInventoryType::AllequipedWeapon, i, true, _ItemInfos, SharedItem);
		}
	}
}

void UInventoryManagerComponent::UpdateAllWeaponInfo(UItemInfo* SharedItem)
{
	int32 FoundIndex = InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.Find(SharedItem);
	bool Equiped = InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.IsValidIndex(FoundIndex);
	
	FindIndex(EBaseInventoryType::AllequipedWeapon, SharedItem, Equiped);
	FindIndex(EBaseInventoryType::Item, SharedItem, Equiped);
	FindIndex(EBaseInventoryType::PrimaryWeapon, SharedItem, Equiped);
	FindIndex(EBaseInventoryType::SecondaryWeapon, SharedItem, Equiped);
	FindIndex(EBaseInventoryType::Sword, SharedItem, Equiped);
	FindIndex(EBaseInventoryType::Staff, SharedItem, Equiped);
	FindIndex(EBaseInventoryType::Shield, SharedItem, Equiped);
}

void UInventoryManagerComponent::FindIndex(EBaseInventoryType InventoryType, UItemInfo* SharedItem, bool Equiped)
{
	int32 ItemIndex = InventoryMap[InventoryType].TypeItemsPtr.Find(SharedItem);
	BroadcastToUpdateWidget(ItemIndex, SharedItem->ItemInfo, Equiped, InventoryType);
}

void UInventoryManagerComponent::RemoveEquipmentItemInfo(EBaseInventoryType InventoryType, int32 Index)
{
	InventoryMap[SelectInventoryType].TypeItemsPtr.Remove(InventoryMap[InventoryType].TypeItemsPtr[Index]);
}

void UInventoryManagerComponent::EmplaceAtSelectEquipmentItem(EBaseInventoryType InventoryType, int32 Index)
{
	InventoryMap[SelectInventoryType].TypeItemsPtr.EmplaceAt(SelectIndex, InventoryMap[InventoryType].TypeItemsPtr[Index]);
}

void UInventoryManagerComponent::Remove_AllWeaponsArray_SelectArray()
{

	if (!InventoryMap[SelectInventoryType].TypeItemsPtr.IsValidIndex(SelectIndex))
	{
		return;
	}

	UItemInfo* SharedItem = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex];
	
	int32 FoundIndex = InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.Find(SharedItem);
	if (FoundIndex == INDEX_NONE)
	{
		return;
	}


	InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.RemoveAt(FoundIndex);
	//InventoryMap[SelectInventoryType].TypeItemsPtr.RemoveAt(SelectIndex);
	InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex] = nullptr;
}

void UInventoryManagerComponent::RemoveSelectItemAndBroadCast()
{
	if (!InventoryMap[SelectInventoryType].TypeItemsPtr.IsValidIndex(SelectIndex))
	{
		return;
	}
	UItemInfo* SharedItem = InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex];
	int32 FoundIndex = InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.Find(SharedItem);
	if (FoundIndex == INDEX_NONE)
	{
		return;
	}
	InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.RemoveAt(FoundIndex);
	InventoryMap[SelectInventoryType].TypeItemsPtr[SelectIndex] = nullptr;

	UpdateAllWeaponInfo(SharedItem);
}

void UInventoryManagerComponent::RemoveComsuneQuantityAtAllItemArray(FName _ItemID)
{
	for (int32 i = 0; i < InventoryMap[EBaseInventoryType::Item].TypeItemsPtr.Num(); ++i)
	{
		if (InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[i]->ItemInfo.ItemID == _ItemID)
		{
			
		}
	}
}

void UInventoryManagerComponent::UpdateQuantityInventory(FName _ItemID, int32 _ItemQuantity)
{
	for (int32 i = 0; i < InventoryMap[EBaseInventoryType::Item].TypeItemsPtr.Num(); ++i)
	{
		if (InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[i]->ItemInfo.ItemID == _ItemID)
		{
			InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[i]->ItemInfo.Quantity = _ItemQuantity;
			BroadcastToUpdateWidget(i, InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[i]->ItemInfo, false, EBaseInventoryType::Item);
		}
	}
	for (int32 i = 0; i < InventoryMap[EBaseInventoryType::Potion].TypeItemsPtr.Num(); ++i)
	{
		if (InventoryMap[EBaseInventoryType::Potion].TypeItemsPtr[i]->ItemInfo.ItemID == _ItemID)
		{
			InventoryMap[EBaseInventoryType::Potion].TypeItemsPtr[i]->ItemInfo.Quantity = _ItemQuantity;
			BroadcastToUpdateWidget(i, InventoryMap[EBaseInventoryType::Potion].TypeItemsPtr[i]->ItemInfo, false, EBaseInventoryType::Potion);
		}
	}
	for (int32 i = 0; i < InventoryMap[EBaseInventoryType::SummonHorse].TypeItemsPtr.Num(); ++i)
	{
		if (InventoryMap[EBaseInventoryType::SummonHorse].TypeItemsPtr[i]->ItemInfo.ItemID == _ItemID)
		{
			InventoryMap[EBaseInventoryType::SummonHorse].TypeItemsPtr[i]->ItemInfo.Quantity = _ItemQuantity;
			BroadcastToUpdateWidget(i, InventoryMap[EBaseInventoryType::SummonHorse].TypeItemsPtr[i]->ItemInfo, false, EBaseInventoryType::SummonHorse);
		}
	}
	for (int32 i = 0; i < InventoryMap[EBaseInventoryType::Consume].TypeItemsPtr.Num(); ++i)
	{
		if (InventoryMap[EBaseInventoryType::Consume].TypeItemsPtr[i] != nullptr)
		{
			if (InventoryMap[EBaseInventoryType::Consume].TypeItemsPtr[i]->ItemInfo.ItemID == _ItemID)
			{
				InventoryMap[EBaseInventoryType::Consume].TypeItemsPtr[i]->ItemInfo.Quantity = _ItemQuantity;
				BroadcastToUpdateWidget(i, InventoryMap[EBaseInventoryType::Consume].TypeItemsPtr[i]->ItemInfo, false, EBaseInventoryType::Consume);
			}
		}
	}
	for (int32 i = 0; i < ConsumeEquipSlots.Num(); i++)
	{
		if (ConsumeEquipSlots[i].ConsumeID == _ItemID)
		{
			ConsumeEquipSlots[i].Quantity = _ItemQuantity;
		}
	}
}




int32 UInventoryManagerComponent::GetSpellMaxIndex()
{
	return SpellEquipInventorySlots.Num();
}

FName UInventoryManagerComponent::GetSpellNameFromIndex(int32 index)
{
	return SpellEquipInventorySlots[index];
}

TArray<FName> UInventoryManagerComponent::GetSpellEquipInventorySlots()
{
	return SpellEquipInventorySlots;
}





void UInventoryManagerComponent::startUpInventerManager()
{
	//SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SpellEquipInventorySlots.Init(NAME_None,10);
	
	// // Bind overlap event
	// OnComponentBeginOverlap.AddDynamic(this, &UInventoryManagerComponent::OnSphereBeginOverlap);
	//
	//
	//
	//
	//
	//
	//
	// SetSphereRadius(200.0f);
	// SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// SetCollisionObjectType(ECC_GameTraceChannel1); // 'ItemCollision'의 Object Type
	// SetCollisionResponseToAllChannels(ECR_Ignore);
	//SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // 아이템 콜리전 채널에만 반응

	
	InventoryMap.Emplace(EBaseInventoryType::Sword, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::Shield, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::Staff, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::Armor, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::Memory, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::Material, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::Consume, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::Potion, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::SummonHorse, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::PrimaryWeapon, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::SecondaryWeapon, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::Item, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::AllequipedWeapon, FItemInfoArray());
	InventoryMap.Emplace(EBaseInventoryType::MakeItem, FItemInfoArray());
	InventoryMap[EBaseInventoryType::PrimaryWeapon].TypeItemsPtr.SetNumZeroed(10);
	InventoryMap[EBaseInventoryType::SecondaryWeapon].TypeItemsPtr.SetNumZeroed(10);
	InventoryMap[EBaseInventoryType::Consume].TypeItemsPtr.SetNumZeroed(10);

	
	FItemInfos RedPotion;
	RedPotion.ItemID = "100";
	RedPotion.ItemTypeID = "1";
	RedPotion.Quantity = 3;
	AddToInventoryWithItemInfo(RedPotion);

	FItemInfos ManaPotion;
	ManaPotion.ItemID = "101";
	ManaPotion.ItemTypeID = "1";
	ManaPotion.Quantity = 3;
	AddToInventoryWithItemInfo(ManaPotion);
	
	// FItemInfos Horse;
	// Horse.ItemID = "102";
	// Horse.ItemTypeID = "1";
	// Horse.Quantity = 1;
	// AddToInventoryWithItemInfo(Horse);
	
	FItemInfos FirstSword;
	FirstSword.ItemID = "1";
	FirstSword.ItemTypeID = "1";
	FirstSword.Quantity = 1;
	AddToInventoryWithItemInfo(FirstSword);

	FItemInfos ThirdSword;
	ThirdSword.ItemID = "3";
	ThirdSword.ItemTypeID = "1";
	ThirdSword.Quantity = 1;
	AddToInventoryWithItemInfo(ThirdSword);
	FItemInfos MakeItem;
	MakeItem.ItemID = "103";
	MakeItem.ItemTypeID = "1";
	MakeItem.Quantity = 10;
	AddToInventoryWithItemInfo(MakeItem);

	FItemInfos Spell;
	Spell.ItemID = "10";
	Spell.ItemTypeID = "1";
	Spell.Quantity = 1;
	AddToInventoryWithItemInfo(Spell);
	Spell.ItemID = "11";
	AddToInventoryWithItemInfo(Spell);
	Spell.ItemID = "12";
	AddToInventoryWithItemInfo(Spell);
	Spell.ItemID = "13";
	AddToInventoryWithItemInfo(Spell);
	Spell.ItemID = "14";
	AddToInventoryWithItemInfo(Spell);
	//Spell.ItemID = "15";
	//AddToInventoryWithItemInfo(Spell);


	FItemInfos Staff;
	Staff.ItemID = "6";
	Staff.ItemTypeID = "1";
	Staff.Quantity = 1;
	AddToInventoryWithItemInfo(Staff);

	FItemInfos Shield;
	Shield.ItemID = "4";
	Shield.ItemTypeID = "1";
	Shield.Quantity = 1;
	AddToInventoryWithItemInfo(Shield);
	
	Staff.ItemID = "7";
	AddToInventoryWithItemInfo(Staff);


	FItemInfos Fruit;
	Fruit.ItemID = "201";
	Fruit.ItemTypeID = "1";
	Fruit.Quantity = 10;
	AddToInventoryWithItemInfo(Fruit);
	SpellEquipInventorySlots.Init(NAME_None,10);



	
}




void UInventoryManagerComponent::FindLeftLightWeaponExist(int32 Index)
{
	TObjectPtr<UItemInfo> Item = InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr[Index];
	int32 FoundIndexRight = InventoryMap[EBaseInventoryType::PrimaryWeapon].TypeItemsPtr.Find(Item);
	if (FoundIndexRight != -1)
	{
		TestIndex = FoundIndexRight;
		ItemID = Item->ItemInfo.ItemID;
		ReinforceID = Item->ItemInfo.ItemTypeID;
		Type = ESpawnWeaponType::Right;
		return;
	}
		
	int32 FoundIndexLeft = InventoryMap[EBaseInventoryType::SecondaryWeapon].TypeItemsPtr.Find(Item);
	if (FoundIndexLeft != -1)
	{
		TestIndex = FoundIndexLeft;
		ItemID = Item->ItemInfo.ItemID;
		ReinforceID = Item->ItemInfo.ItemTypeID;
		Type = ESpawnWeaponType::Left;
	}
}

void UInventoryManagerComponent::BroadcastOnPressedQ()
{
	OnPressedQ.Broadcast();
}


void UInventoryManagerComponent::CachedSelf()
{
	UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	Subsystem->CachedInventoryManager = this; 
}

void UInventoryManagerComponent::AddToConsume(UItemInfo* SharedItem)
{
	InventoryMap[EBaseInventoryType::Consume].TypeItemsPtr.Emplace(SharedItem);
}

void UInventoryManagerComponent::UpdateAllequipedWeapon()
{

	for (int32 i = 0; i < InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.Num(); ++i)
	{
		if (InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr.IsValidIndex(i))
		{
			if (InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr[i] != nullptr)
			{
				UpdateAllWeaponInfo(InventoryMap[EBaseInventoryType::AllequipedWeapon].TypeItemsPtr[i]);
			}
		}
	}
}