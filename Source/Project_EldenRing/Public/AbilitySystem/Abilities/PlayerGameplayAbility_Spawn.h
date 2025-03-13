// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"
#include "BaseType/BaseStruct.h"
#include "PlayerGameplayAbility_Spawn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPlayerGameplayAbility_Spawn : public UPlayerGameplayAbility
{
	GENERATED_BODY()

#pragma region Protected
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion
private:
	UFUNCTION(BlueprintCallable)
	void ChangedWeapon();

	UFUNCTION(BlueprintCallable)
	void SpawnNewWeapon();
	
	UPROPERTY(EditDefaultsOnly,Category="SocketName")
	FName SocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly,Category="SocketName")
	FName HandSocketName;
	
	FAttachmentTransformRules AttachmentRules = {EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,true};
	
	UPROPERTY(EditDefaultsOnly,Category="ChangedInventory")
	TMap<int32, APlayerWeaponBase*> SelectedInventory;
	
	FPlayerWeaponData WeaponData;
	//FWeaponStatisticsDataInfo PlayerWeaponReinforceData;
	ESpawnWeaponType CurrentType;
	FString Item_ID;
	FString Reinforce_ID;
	FString RowName;
	int32 CurrentEquipIndex;
	int32 Slot;

	bool bIsEquipped = false;
	
};
