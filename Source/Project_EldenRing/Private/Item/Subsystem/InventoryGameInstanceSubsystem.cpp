// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Subsystem/InventoryGameInstanceSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "Components/Inventory/InventoryManagerComponent.h"

void UInventoryGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

UInventoryGameInstanceSubsystem::UInventoryGameInstanceSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DroppedDTAsset(TEXT("/Game/_BP/Item/ItemDataTable/DroppedItemDataTable/DroppedItemInfoTable.DroppedItemInfoTable"));
	if (DroppedDTAsset.Succeeded())
	{
		DroppedTable = DroppedDTAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load WeaponDataTable in constructor."));
	}

	SetDroppedItemData();
	
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponStatistic(TEXT("/Game/_BP/Item/ItemDataTable/WeaponStatisicDataTable/WeaponStatisticsDataInfo.WeaponStatisticsDataInfo"));
	if (WeaponStatistic.Succeeded())
	{
		WeaponStatisticsTable = WeaponStatistic.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load WeaponDataTable in constructor."));
	}

	SetWeaponStatisticsData();
	
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerRightWeaponDT(TEXT("/Game/_BP/Item/ItemDataTable/PlayerWeaponDataTable/PlayerWeaponRightData.PlayerWeaponRightData"));
	if (PlayerRightWeaponDT.Succeeded())
	{
		PlayerRightWeaponDataTable = PlayerRightWeaponDT.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load WeaponDataTable in constructor."));
	}

	SetPlayerRightWeaponData();
	
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerLeftWeaponDT(TEXT("/Game/_BP/Item/ItemDataTable/PlayerWeaponDataTable/PlayerWeaponLeftData.PlayerWeaponLeftData"));
	if (PlayerLeftWeaponDT.Succeeded())
	{
		PlayerLeftWeaponDataTable = PlayerLeftWeaponDT.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load WeaponDataTable in constructor."));
	}
	
	SetPlayerLeftWeaponData();

	static ConstructorHelpers::FObjectFinder<UDataTable> InventorySpellInfoDT(TEXT("/Game/_BP/Item/ItemDataTable/SpellDataTable/InventorySpellInfoTable.InventorySpellInfoTable"));
	if (InventorySpellInfoDT.Succeeded())
	{
		SpellInventoryDataTable = InventorySpellInfoDT.Object;
		SetInventorySpellData();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load InventorySpellTable in constructor."));
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerSpellDataDT(TEXT("/Game/_BP/Item/ItemDataTable/SpellDataTable/PlayerSpell_Info_Table.PlayerSpell_Info_Table"));
	if (PlayerSpellDataDT.Succeeded())
	{
		PlayerSpellDataTable = PlayerSpellDataDT.Object;
		SetPlayerSpellData();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load PlayerSpellTable in constructor."));
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> AshofWarDT(TEXT("/Game/_BP/Item/ItemDataTable/AshOfWarDataTable/AshOfWarInfoDataTable.AshOfWarInfoDataTable"));
	if (AshofWarDT.Succeeded())
	{
		AshOfWarDataTable = AshofWarDT.Object;
		SetAshOfWarData();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load PlayerSpellTable in constructor."));
	}
	
	
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDTAsset(TEXT("/Game/_BP/Item/ItemDataTable/WeaponDataTable.WeaponDataTable"));
	if (WeaponDTAsset.Succeeded())
	{
		WeaponDataTable = WeaponDTAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load WeaponDataTable in constructor."));
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponLeftDTAsset(TEXT("/Game/_BP/Item/ItemDataTable/WeaponLeftDataTable.WeaponLeftDataTable"));
	if (WeaponLeftDTAsset.Succeeded())
	{
		WeaponLeftDataTable = WeaponLeftDTAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load WeaponDataTable in constructor."));
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> HorseDTAsset(TEXT("/Game/_BP/Item/ItemDataTable/ConsumeDataTable/HorseDataTable.HorseDataTable"));
	if (HorseDTAsset.Succeeded())
	{
		HorseDataTable = HorseDTAsset.Object;
		SetHorseData();

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load HorseDataTable in constructor."));
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> PotionDTAsset(TEXT("/Game/_BP/Item/ItemDataTable/ConsumeDataTable/PotionDataTable.PotionDataTable"));
	if (PotionDTAsset.Succeeded())
	{
		PotionDataTable = PotionDTAsset.Object;
		SetPotionData();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load HorseDataTable in constructor."));
	}


	

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDTAsset(TEXT("/Game/_BP/Item/ItemDataTable/ItemDataTable.ItemDataTable"));
	if (ItemDTAsset.Succeeded())
	{
		ItemDataTable = ItemDTAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load ItemDataTable in constructor."));
	}
	
	if (ItemDataTable)
	{
		
		TArray<FName> RowNames = ItemDataTable->GetRowNames();

		for (const FName& RowName : RowNames)
		{
			FItemData* RowData = ItemDataTable->FindRow<FItemData>(RowName, TEXT("GetAllRows"));
			if (RowData)
			{
				Items.Add(RowName, *RowData);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Row not found: %s"), *RowName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is null"));
	}
	


	
}

#pragma region InitSetter
void UInventoryGameInstanceSubsystem::SetDroppedItemData()
{
	if (DroppedTable)
	{
		TArray<FName> DroppedRowNames = DroppedTable->GetRowNames();
		for (const FName& DataName : DroppedRowNames)
		{
			if (FDroppedItemInfo* RowData = DroppedTable->FindRow<FDroppedItemInfo>(DataName, TEXT("Find")))
			{
				DroppedData.Add(DataName,*RowData);
			}
		}
	}
}

void UInventoryGameInstanceSubsystem::SetWeaponStatisticsData()
{
	if (WeaponStatisticsTable)
	{
		TArray<FName> RowNames = WeaponStatisticsTable->GetRowNames();
		for (const FName& DataName : RowNames)
		{
			if (FWeaponStatisticsDataInfo* RowData = WeaponStatisticsTable->FindRow<FWeaponStatisticsDataInfo>(DataName, TEXT("Find")))
			{
				WeaponStatisticsData.Add(DataName,*RowData);
			}
		}
	}
}

void UInventoryGameInstanceSubsystem::SetPlayerRightWeaponData()
{
	if (PlayerRightWeaponDataTable)
	{
		TArray<FName> RowNames = PlayerRightWeaponDataTable->GetRowNames();
		for (const FName& DataName : RowNames)
		{
			if (FPlayerWeaponData* RowData = PlayerRightWeaponDataTable->FindRow<FPlayerWeaponData>(DataName, TEXT("Find")))
			{
				PlayerRightWeaponData.Add(DataName,*RowData);
			}
		}
	}
}

void UInventoryGameInstanceSubsystem::SetPlayerLeftWeaponData()
{
	if (PlayerLeftWeaponDataTable)
	{
		TArray<FName> RowNames = PlayerLeftWeaponDataTable->GetRowNames();
		for (const FName& DataName : RowNames)
		{
			if (FPlayerWeaponData* RowData = PlayerLeftWeaponDataTable->FindRow<FPlayerWeaponData>(DataName, TEXT("Find")))
			{
				PlayerLeftWeaponData.Add(DataName,*RowData);
			}
		}
	}
}

void UInventoryGameInstanceSubsystem::SetInventorySpellData()
{
	if (SpellInventoryDataTable)
	{
		TArray<FName> RowNames = SpellInventoryDataTable->GetRowNames();
		for (const FName& DataName : RowNames)
		{
			if (FInventorySpellInfo* RowData = SpellInventoryDataTable->FindRow<FInventorySpellInfo>(DataName, TEXT("Find")))
			{
				SpellInventoryData.Add(DataName,*RowData);
			}
		}
	}
}

void UInventoryGameInstanceSubsystem::SetPlayerSpellData()
{
	if (PlayerSpellDataTable)
	{
		TArray<FName> RowNames = PlayerSpellDataTable->GetRowNames();
		for (const FName& DataName : RowNames)
		{
			if (FPlayerSpellInfo* RowData = PlayerSpellDataTable->FindRow<FPlayerSpellInfo>(DataName, TEXT("Find")))
			{
				PlayerSpellData.Add(DataName,*RowData);
			}
		}
	}
}

void UInventoryGameInstanceSubsystem::SetAshOfWarData()
{
	if (AshOfWarDataTable)
	{
		TArray<FName> RowNames = AshOfWarDataTable->GetRowNames();
		for (const FName& DataName : RowNames)
		{
			if (FAshOfWarInfo* RowData = AshOfWarDataTable->FindRow<FAshOfWarInfo>(DataName, TEXT("Find")))
			{
				AshOfWarData.Add(DataName,*RowData);
			}
		}
	}
}

void UInventoryGameInstanceSubsystem::SetHorseData()
{
	if (HorseDataTable)
	{
		TArray<FName> RowNames = HorseDataTable->GetRowNames();
		for (const FName& DataName : RowNames)
		{
			if (FHorseInfo* RowData = HorseDataTable->FindRow<FHorseInfo>(DataName, TEXT("Find")))
			{
				HorseData.Add(DataName,*RowData);
			}
		}
	}
}

void UInventoryGameInstanceSubsystem::SetPotionData()
{
	if (PotionDataTable)
	{
		TArray<FName> RowNames = PotionDataTable->GetRowNames();
		for (const FName& DataName : RowNames)
		{
			if (FPotionInfo* RowData = PotionDataTable->FindRow<FPotionInfo>(DataName, TEXT("Find")))
			{
				PotionData.Add(DataName,*RowData);
			}
		}
	}
}
#pragma endregion 

#pragma region Getter
FInventorySpellInfo UInventoryGameInstanceSubsystem::GetSpellInventoryData(FName RowName)
{
	checkf(SpellInventoryData.Find(RowName),TEXT("Player Spell Table is NULL"));

	return *SpellInventoryData.Find(RowName);
}

FPlayerSpellInfo UInventoryGameInstanceSubsystem::GetPlayerSpellData(FName RowName)
{
	checkf(PlayerSpellData.Find(RowName),TEXT("Player Spell Table is NULL"));

	return *PlayerSpellData.Find(RowName);
}

FAshOfWarInfo UInventoryGameInstanceSubsystem::GetAshOfWarData(FName RowName)
{
	checkf(AshOfWarData.Find(RowName),TEXT("Can not found AshOfWarData by RowName"));
	return *AshOfWarData.Find(RowName);
}



FPlayerWeaponData UInventoryGameInstanceSubsystem::GetWeaponRightRowData(FName RowName)
{
	checkf(PlayerRightWeaponData.Find(RowName),TEXT("Can't Find PlayerRightWeaponData Row From RowName"));
		
	return *PlayerRightWeaponData.Find(RowName);
}

FPlayerWeaponData UInventoryGameInstanceSubsystem::GetWeaponLeftRowData(FName RowName)
{
	checkf(PlayerLeftWeaponData.Find(RowName),TEXT("Can't Find PlayerLeftWeaponData Row From RowName"));
		
	return *PlayerLeftWeaponData.Find(RowName);
}

FDroppedItemInfo UInventoryGameInstanceSubsystem::GetDroppedItemData(FName RowName)
{
	if (FDroppedItemInfo* FoundData = DroppedData.Find(RowName))
	{
		return *FoundData;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Can't Find DroppedItemData for RowName: %s"), *RowName.ToString());
		return FDroppedItemInfo(); 
	}
}

FWeaponStatisticsDataInfo UInventoryGameInstanceSubsystem::GetWeaponStatisticsDataInfoData(FName RowName)
{
	if (!WeaponStatisticsData.Contains(RowName))
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find WeaponStatisticsData: %s"), *RowName.ToString());
		return FWeaponStatisticsDataInfo(); 
	}

	return *WeaponStatisticsData.Find(RowName);
}



FHorseInfo UInventoryGameInstanceSubsystem::GetHorseData(FName RowName)
{
	checkf(HorseData.Find(RowName),TEXT("Can't Find HorseData Row From RowName"));
		
	return *HorseData.Find(RowName);
}

FPotionInfo UInventoryGameInstanceSubsystem::GetPotionData(FName RowName)
{
	checkf(PotionData.Find(RowName),TEXT("Can't Find PotionData Row From RowName"));
		
	return *PotionData.Find(RowName);
}


void UInventoryGameInstanceSubsystem::SetCachedInventToCurrentInventoryManager()
{
	UE_LOG(LogTemp, Warning, TEXT("SetCachedInventToCurrentInventoryManager CALLED"));
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APawn* Pawn = PC->GetPawn();
		APlayerCharacter* MyChar = Cast<APlayerCharacter>(Pawn);
		if (MyChar)
		{
			MyChar->SetInventoryManager(CachedInventoryManager);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't Set InventoryManager"));
		}
		UE_LOG(LogTemp, Warning, TEXT("Pawn is: %s"), *Pawn->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Pawn class is: %s"), *Pawn->GetClass()->GetName());
		FName Name = MyChar->GetInventoryManager()->InventoryMap[EBaseInventoryType::Item].TypeItemsPtr[0]->ItemInfo.ItemID;
		UE_LOG(LogTemp, Warning, TEXT("Pawn class is: %s"), *Name.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Set InventoryManagerPC"));
	}
}

#pragma endregion


