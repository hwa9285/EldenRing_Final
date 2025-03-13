// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HumanCombatComponent.generated.h"

class IPawnUIInterface;
class UInventoryGameInstanceSubsystem;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UHumanCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UHumanCombatComponent();
	
#pragma region Weapon combat
	virtual void OnHitTargetActor(AActor* HitActor) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetHeroCurrentEquippedWeaponDamageAtLevel(float Level) const;
	
	UFUNCTION(BlueprintCallable, Category = "Player_Combat")
	void TogglePlayerWeaponCollision(bool bUse, EToggleDamageType ToggleDamagetype = EToggleDamageType::CurrentEquippedWeapon);

	UFUNCTION(BlueprintCallable, Category = "Player_Combat")
	void TogglePlayerLeftWeaponCollision(bool bUse, EToggleDamageType ToggleDamagetype = EToggleDamageType::CurrentEquippedWeapon);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Player_Combat")
	TMap<int32, APlayerWeaponBase*> GetCurrentCarriedMapByType(ESpawnWeaponType Type);
	
	UFUNCTION(BlueprintCallable, Category = "Player_Combat")
	void RegisterPlayerWeaponSpawn(int32 SlotIndex, ESpawnWeaponType Type, APlayerWeaponBase* Weapon, bool bRegisterAsPlayerEquippedWeapon);

	UFUNCTION(BlueprintCallable, Category = "Player_Combat")
	void RemovePlayerWeapon(int32 SlotIndex,ESpawnWeaponType _Type);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerEquipWeapon")
	FGameplayTag CurrentEquipRightWeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerEquipWeapon")
	FGameplayTag CurrentEquipLeftWeaponTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PlayerEquipWeapon")
	int32 CurrentEquipRightWeaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PlayerEquipWeapon")
	int32 CurrentEquipLeftWeaponIndex;
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Player_Combat")
	APlayerWeaponBase* GetCurrentEquipRightWeaponByIndex(int32 SlotIndex);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Player_Combat")
	APlayerWeaponBase* GetCurrentEquipLeftWeaponByIndex(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Player_Combat")
	TMap<int32, APlayerWeaponBase*>& GetPlayerCarriedPrimaryWeaponMap(){return PlayerCarriedPrimaryWeaponMap;}

	UFUNCTION(BlueprintCallable,BlueprintPure ,Category = "Player_Combat")
	TMap<int32, APlayerWeaponBase*>& GetPlayerCarriedSecondaryWeaponMap(){return PlayerCarriedSecondaryWeaponMap;}
#pragma endregion

#pragma region SpellCombat
	UFUNCTION(BlueprintCallable, Category = "Player_Spell_Combat")
	void RegisterCarriedSpell(uint8 Slot, FName SpellID);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Player_Spell_Combat")
	TArray<FName> GetPlayerCarriedSpells(){return PlayerCarriedSpell;};
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Player_Spell_Combat")
	FName GetCurrentEquipSpellID();

	//무기를 장착하거나 스펠을 장착할 때 성공 여부를 캐싱하고 저장하는 함수
	UFUNCTION(BlueprintCallable, Category = "Player_Spell_Combat")
	void UpdateSpellExecutionData();

	UFUNCTION(BlueprintCallable, Category = "Player_Spell_Combat")
	void IsEnoughFP();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player_Spell_Combat")
	FSpellExecutionData GetCurrentSpellExecutionData(){return CurrentSpellExecutionData;}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PlayerEquipWeapon")
	uint8 CurrentEquipSpellIndex;
#pragma endregion

#pragma region ItemCombat
	//아이템 등록
	UFUNCTION(BlueprintCallable, Category = "Player Item Combat")
	void RegisterItem(uint8 SlotIndex, FConsumeInfo ItemInfo);

	//아이템 사용 전, 사용 가능한지 체크하는 함수
	UFUNCTION(BlueprintCallable, Category = "Player Item Combat")
	bool CheckAvailableItem();

	//착용시 위젯 업데이트
	UFUNCTION(BlueprintCallable, Category = "Player Item Combat")
	void UpdateEquipItemWidget();

	//사용할 때, 개수 반영하는 함수
	UFUNCTION(BlueprintCallable, Category = "Player Item Combat")
	void UpdateCurrentQuantity();

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Player Item Combat")
	TArray<FConsumeInfo> GetCarriedItem(){return CarriedItem;};

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Player Item Combat")
	uint8 CurrentEquipItemIndex;
	
#pragma endregion

#pragma region SaveData
	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	void SaveData();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	TMap<int32, APlayerWeaponBase*> GetCarriedPrimaryWeaponMap(){return PlayerCarriedPrimaryWeaponMap;}

	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	TMap<int32, APlayerWeaponBase*> GetCarriedSecondaryWeaponMap(){return PlayerCarriedSecondaryWeaponMap;}

	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	void SetCarriedPrimaryWeaponMap(TMap<int32, APlayerWeaponBase*> SaveDataMap){PlayerCarriedPrimaryWeaponMap = SaveDataMap;}

	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	void SetCarriedSecondaryWeaponMap(TMap<int32, APlayerWeaponBase*> SaveDataMap){PlayerCarriedSecondaryWeaponMap = SaveDataMap;}

	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	void SetCarriedSpellArray(TArray<FName> SaveData){PlayerCarriedSpell = SaveData;}

	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	void SetCarriedItem(TArray<FConsumeInfo> SaveData){CarriedItem = SaveData;}

	//플레이어가 부활할 때, 무기의 Owner를 지정하고 소켓에 부착하는 함수
	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	void RegisterPlayerWeaponByNewPawn();

	UFUNCTION(BlueprintCallable, Category = "Combat Save")
	void InitializeEquipItem();
	
#pragma endregion
	
	TWeakInterfacePtr<IPawnUIInterface> GetPawnUIInterface(){return CachedUIInterface;};

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedUIInterface;
	
	FSpellExecutionData CurrentSpellExecutionData;
	
	TMap<int32, APlayerWeaponBase*> PlayerCarriedPrimaryWeaponMap;
	TMap<int32, APlayerWeaponBase*> PlayerCarriedSecondaryWeaponMap;
	
	TArray<FName> PlayerCarriedSpell;

	TArray<FConsumeInfo> CarriedItem;
	
};

