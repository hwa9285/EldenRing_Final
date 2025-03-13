// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/BaseStruct.h"
#include "Interfaces/ItemPickupInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

class UStatComponent;
class UPlayerAttributeSet;
class APlayerWeaponBase;

USTRUCT(BlueprintType)
struct FCombatSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PlayerEquipWeapon")
	int32 SaveEquipRightWeaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PlayerEquipWeapon")
	int32 SaveEquipLeftWeaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PlayerEquipWeapon")
	TMap<int32, APlayerWeaponBase*> SaveCarriedPrimaryWeaponMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PlayerEquipWeapon")
	TMap<int32, APlayerWeaponBase*> SaveCarriedSecondaryWeaponMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PlayerEquipWeapon")
	uint8 SaveEquipSpellIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PlayerEquipWeapon")
	TArray<FName> SaverCarriedSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PlayerEquipWeapon")
	uint8 SaveEquipItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PlayerEquipWeapon")
	TArray<FConsumeInfo> SaveCarriedItem;
};
USTRUCT(BlueprintType)
struct FSaveAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SaveAttribute")
	float MaxHP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SaveAttribute")
	float MaxStamina;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SaveAttribute")
	float MaxFP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SaveAttribute")
	float BaseAttack;
};

USTRUCT(BlueprintType)
struct FSaveStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	TMap<EStatsType,uint8> SaveBaseStat;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Stat")
	uint8 SaveCurrentLevel;
};

USTRUCT(BlueprintType)
struct FSaveInventory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<EBaseInventoryType, FItemInfoArray> SaveInventoryMap;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Spell Inventory")
	TArray<FName> SaveSpellEquipInventorySlots;
	
	UPROPERTY(BlueprintReadWrite ,Category = "Consume Inventory")
	TArray<FConsumeInfo> SaveConsumeEquipSlots;
};
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USaveSubsystem();
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category = "SaveSubsystem")
	FCombatSaveData SaveCombatData;

	UPROPERTY(Blueprintable, BlueprintReadWrite, Category = "SaveSubsystem")
	FSaveAttribute SaveAttributeData;

	UPROPERTY(BlueprintReadWrite, Category = "SaveSubsystem")
	FSaveStat SaveStatData;

	UPROPERTY(BlueprintReadWrite, Category = "SaveSubsystem")
	FSaveInventory SaveInventoryData;
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category = "SaveSubsystem")
	FTransform RespawnTransform;
	
	UFUNCTION(BlueprintCallable, Category = "SaveSubsystem")
	void SaveData(APlayerCharacter* PlayerCharacter);

	//플레이어가 죽을때 기준
	UFUNCTION(BlueprintCallable, Category = "SaveSubsystem")
	void SaveDataByPlayerDeath(APlayerCharacter* PlayerCharacter);
	
	UFUNCTION(BlueprintCallable, Category = "SaveSubsystem")
	void SaveCombatComponent(UHumanCombatComponent* SaveCombatComponent);

	UFUNCTION(BlueprintCallable, Category = "SaveSubsystem")
	void SaveAttributeSet(UPlayerAttributeSet* SaveAttributeSet);

	UFUNCTION(BlueprintCallable, Category = "SaveSubsystem")
	void SaveStatComponent(UStatComponent* SaveStatComponent);

	UFUNCTION(BlueprintCallable, Category = "SaveSubsystem")
	void SaveInventoryComponent(UInventoryManagerComponent* SaveInventoryComponent);
	
};