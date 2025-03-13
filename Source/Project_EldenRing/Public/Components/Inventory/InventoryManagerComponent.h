// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BaseType/BaseStruct.h"
#include "Character/PlayerCharacter.h"
#include "Components/PawnExtensionComponent.h"
#include "Interfaces/ItemPickupInterface.h"
#include "Item/SpawnItem/DroppedItemBase.h"
#include "InventoryManagerComponent.generated.h"


class UWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnChangedItemDelegate, int32, Index, FItemInfos, ItemInfos, bool,
                                              Equiped,
                                              EBaseInventoryType, EquipType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHoverEventDelegate, FItemInfos, ItemInfos, EBaseInventoryType, EquipType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetInteractName, FString, Name);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetInteractName, FName, Name);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddtoInventory);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressedQ);


struct FItemInfos;
class UDataAsset_SpellBase;
class AItemBase;
class APlayerWeaponBase;
class UWidgetSwitcher;

UCLASS()
class PROJECT_ELDENRING_API UInventoryManagerComponent : public UActorComponent 
{
	GENERATED_BODY()

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:


	UInventoryManagerComponent();
	
	FItemData Row;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	AActor* CurrentInteractItem;
	
	// // Handle overlap event
	// UFUNCTION()
	// void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	// 						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	// 						  const FHitResult& SweepResult);



	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FItemInfos ItemInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FGuid Guid;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName CurrentHoverItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 SelectIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxSpellIndex = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 CurrentSpellIndex = 0;

	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EBaseInventoryType SelectInventoryType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EInteractType SelectInteractType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemInfos> AllMemory;
	
	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FOnChangedItemDelegate ChangedItemDelegate;

	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FOnHoverEventDelegate OnHoverEventDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSetInteractName OnSetInteractName;

	//UPROPERTY(BlueprintAssignable)
	FOnGetInteractName OnGetInteractName;
	
	UPROPERTY(BlueprintAssignable)
	FOnAddtoInventory OnAddtoInventory;

	UPROPERTY(BlueprintAssignable)
	FOnPressedQ OnPressedQ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	UWidgetSwitcher* MyWidgetSwitcherReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	UWidgetSwitcher* EnhanceShopWidgetSwitcherReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<EBaseInventoryType, FItemInfoArray> InventoryMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FItemInfos EmptyItemInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInhencedInt = 7;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	// UInventotySystemComponent* InventorySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxHealthPotionInt = 3;

	
	
	FTimerHandle OnOffCollisioinTimerHandle;
	
	UFUNCTION(BlueprintCallable)
    void AddToInventory(AActor* InteractItem);

	UFUNCTION(BlueprintCallable)
	void AddToInventoryWithItemInfo(FItemInfos _ItemInfos);

	UFUNCTION(BlueprintCallable)
    void DropItem(int32 ItemIndex, TSubclassOf<ADroppedItemBase> DroppedItemClass);
	
	UFUNCTION(BlueprintCallable)
	void BroadcastToUpdateWidget(int32 Index, FItemInfos _ItemInfos,bool Equiped, EBaseInventoryType EquipType);
	
	UFUNCTION(BlueprintCallable)
	void BroadCastHoverEvent(FItemInfos _ItemInfos, EBaseInventoryType _EquipType);
	
	UFUNCTION(BlueprintCallable)
	void UpdateAllSlot(TArray<UWidget*> Widgets, EBaseInventoryType _EquipType);

	UFUNCTION(BlueprintCallable)
	void UpdateAllMaxInt(int32 MaxInt, EBaseInventoryType _EquipType);
	UFUNCTION(BlueprintCallable)
	void UpdateAllItem();

	
	UFUNCTION(BlueprintCallable)
	bool FindItemAndSet(int32 _FindIndex, EBaseInventoryType _FindEquipType, EBaseInventoryType& _ReturnEquipType, int32& _ReturnIndex);
	
	UFUNCTION(BlueprintCallable)
	void SwapIfChoiceIsValidInLeftLight(EBaseInventoryType _ReturnEquipType, int32 _ReturnIndex);

	UFUNCTION(BlueprintCallable)
	void SwapEmptySlotToHasSlot(EBaseInventoryType _ReturnEquipType, int32 _ReturnIndex);

	UFUNCTION(BlueprintCallable)
	void SwapFindSlotToHasSlot(EBaseInventoryType _FindEquipType, int32 _FindIndex);
	
	
	//혜민이가 추가한 테스트용 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	ESpawnWeaponType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 TestIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ReinforceID;



	TWeakInterfacePtr<IPawnUIInterface> GetPawnUIInterface(){return CachedUIInterface;};
private:
	TWeakInterfacePtr<IPawnUIInterface> CachedUIInterface;

	
private:
	//마법 장착에 필요한 변수
	UPROPERTY(EditAnywhere, Category = "Spell Inventory")
	FName SpellID;//ItemID와 동일

	UPROPERTY(EditAnywhere, Category = "Spell Inventory")
	uint8 SpellEquipSlot; //인벤토리 위젯에서 클릭했을 때, 그 슬롯의 Index
	
	//마법은 장착로직이 다르므로 배열 필요
	UPROPERTY(EditAnywhere, Category = "Spell Inventory")
	TArray<FName> SpellEquipInventorySlots;

	UFUNCTION(BlueprintCallable,Category="Spell Inventory")
	void SetSpellToInventory(TArray<FName> _SpellEquipInventorySlots)
	{
		SpellEquipInventorySlots = _SpellEquipInventorySlots;
	}

	
	UFUNCTION(BlueprintCallable,Category="Spell Inventory")
	void AddSpellToInventory(uint8 SelectedSlotIndex,FName SelectedSpellID);
	
public:
	// 소비장착에 필요한 변수
	UPROPERTY(BlueprintReadWrite, Category = "Consume Inventory")
	uint8 ConsumeSlot; //인벤토리 위젯에서 클릭했을 때, 그 슬롯의 Index
	
	UPROPERTY(BlueprintReadWrite ,Category = "Consume Inventory")
	TArray<FConsumeInfo> ConsumeEquipSlots;// 구조체(ItemID이름, 수량, 소비타입) 배열
	
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	EConsumeType ConsumeType; //추가 됬을때 그 아이템의 타입?? 말인지 포션인지?


public:
	UFUNCTION(BlueprintCallable)
	void AddToInventoryByNPC(FItemInfos _ItemInfos);
	
	//아이템 사용 가능여부에 대한 함수 
public:

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FName GetSpellID(){return SpellID;};

	UFUNCTION(BlueprintCallable,BlueprintPure)
	uint8 GetSpellEquipSlot(){return SpellEquipSlot;};
	
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerCharacter();


	
	UFUNCTION(BlueprintCallable)
	void PopulateUniformGrid(UUniformGridPanel* InUniformGridPanel, UUserWidget* NewWidget);
	UFUNCTION(BlueprintCallable)
	void CheckQuantity(FItemInfos _ItemInfos, EBaseInventoryType InventoryType);

	UFUNCTION(BlueprintCallable)
	void ActiveCollision();
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemInfos*> AllMemorysd;


	UFUNCTION(BlueprintCallable)
	void AddItemInfoPtr(EBaseInventoryType InventoryType, UItemInfo* SharedItem);

	UFUNCTION(BlueprintCallable)
	void AddItemInfoPtrIndex(EBaseInventoryType InventoryType, UItemInfo* SharedItem, int32 Index);



	
	UFUNCTION(BlueprintCallable)
	bool IsValidGetItemInfo(EBaseInventoryType InventoryType, int32 Index, bool Equiped, FItemInfos& _ItemInfos ,UItemInfo*& SharedItem);


	UFUNCTION(BlueprintCallable)
	void RemoveItemAllequipedWeaponInfoPtr(EBaseInventoryType InventoryType, int32 Index);


	UFUNCTION(BlueprintCallable)
	void UpdateAllWeaponInfo(UItemInfo* SharedItem);


	
	UFUNCTION(BlueprintCallable)
	void FindIndex(EBaseInventoryType InventoryType , UItemInfo* SharedItem, bool Equiped);

	UFUNCTION(BlueprintCallable)
	void RemoveEquipmentItemInfo(EBaseInventoryType InventoryType , int32 Index);

	UFUNCTION(BlueprintCallable)
	void EmplaceAtSelectEquipmentItem(EBaseInventoryType InventoryType , int32 Index);


	UFUNCTION(BlueprintCallable)
	void Remove_AllWeaponsArray_SelectArray();

	
	UFUNCTION(BlueprintCallable)
	int32 GetSpellMaxIndex();

	// UFUNCTION(BlueprintCallable)
	// void UpdateAllSlot(TArray<UWidget*> Widgets, EBaseInventoryType _EquipType);
	

	UFUNCTION(BlueprintCallable)
	FName GetSpellNameFromIndex(int32 index);


	//마법은 장착로직이 다르므로 배열 필요
	UFUNCTION(BlueprintCallable)
	TArray<FName> GetSpellEquipInventorySlots();















	UFUNCTION(BlueprintCallable)
	void FindLeftLightWeaponExist(int32 Index);
	UFUNCTION(BlueprintCallable)
	void BroadcastOnPressedQ();

	UFUNCTION(BlueprintCallable)
	void RemoveSelectItemAndBroadCast();

	UFUNCTION(BlueprintCallable)
	void RemoveComsuneQuantityAtAllItemArray(FName _ItemID);



	UFUNCTION(BlueprintCallable)
	void UpdateQuantityInventory(FName _ItemID, int32 _ItemQuantity);

	UFUNCTION(BlueprintCallable)
	void startUpInventerManager();



	UFUNCTION(BlueprintCallable)
	void CachedSelf();



	UFUNCTION(BlueprintCallable)
	void AddToConsume(UItemInfo* SharedItem);


	UFUNCTION(BlueprintCallable)
	void UpdateAllequipedWeapon();
};



