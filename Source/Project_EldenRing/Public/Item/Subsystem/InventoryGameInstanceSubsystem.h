// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BaseType/BaseStruct.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "InventoryGameInstanceSubsystem.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT_ELDENRING_API UInventoryGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UInventoryGameInstanceSubsystem();


private:

	
	
#pragma region ItemDataTable
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* DroppedTable;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* WeaponStatisticsTable;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* PlayerRightWeaponDataTable;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* PlayerLeftWeaponDataTable;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* HorseDataTable;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* PotionDataTable;
	
#pragma endregion

#pragma region ItemDataMap
	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FDroppedItemInfo> DroppedData;
	
	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FWeaponStatisticsDataInfo> WeaponStatisticsData;

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FPlayerWeaponData> PlayerRightWeaponData;

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FPlayerWeaponData> PlayerLeftWeaponData;

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FPlayerWeaponData> InteractTalkData;

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FHorseInfo> HorseData;

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FPotionInfo> PotionData;
#pragma endregion

#pragma region SpellDataTable
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* SpellInventoryDataTable;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* PlayerSpellDataTable;
#pragma endregion

#pragma region AshOfWarData
	
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* AshOfWarDataTable;

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FAshOfWarInfo> AshOfWarData;
	
#pragma endregion 	
	
#pragma region SpellDataMap
	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FInventorySpellInfo> SpellInventoryData;

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FName, FPlayerSpellInfo> PlayerSpellData;
#pragma endregion
	//여기까지
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* ItemDataTable;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* WeaponLeftDataTable;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<FName, FItemData> Items;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 SelectRightWeaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 SelectLeftWeaponIndex;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	// FPotionInfo HealthPotion;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	// FPotionInfo ManaPotion;

	
	UFUNCTION(BlueprintCallable, Category = "Data")
	FItemData GetItemRowData(FName RowName, const FString& ContextString = TEXT("Lookup Item"))
	{
		if (ItemDataTable)
		{
			return *ItemDataTable->FindRow<FItemData>(RowName, TEXT("Lookup Item"));
		}
		return FItemData();
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGameStart = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryManagerComponent* CachedInventoryManager;
	
	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetCachedInventToCurrentInventoryManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetBase* CachedWidgetInstance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<EBaseInventoryType, FItemInfoArray> SaVEInventoryMap;
	
#pragma region Getter
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Data")
	FPlayerWeaponData GetWeaponRightRowData(FName RowName);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Data")
	FPlayerWeaponData GetWeaponLeftRowData(FName RowName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Data")
	FDroppedItemInfo GetDroppedItemData(FName RowName);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Data")
	FWeaponStatisticsDataInfo GetWeaponStatisticsDataInfoData(FName RowName);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Data")
	FInventorySpellInfo GetSpellInventoryData(FName RowName);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Data")
	FPlayerSpellInfo GetPlayerSpellData(FName RowName);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Data")
	FAshOfWarInfo GetAshOfWarData(FName RowName);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Data")
	FHorseInfo GetHorseData(FName RowName);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Data")
	FPotionInfo GetPotionData(FName RowName);

	
#pragma endregion
	
#pragma region Setter
	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetDroppedItemData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetWeaponStatisticsData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetPlayerRightWeaponData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetPlayerLeftWeaponData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetInventorySpellData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetPlayerSpellData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetAshOfWarData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetHorseData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetPotionData();
	
#pragma endregion
	
};


