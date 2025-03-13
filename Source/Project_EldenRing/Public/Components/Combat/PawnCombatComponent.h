// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseType/BaseStruct.h"
#include "Components/PawnExtensionComponent.h"
#include "PawnCombatComponent.generated.h"

class AWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
};

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPawnCombatComponent : public UPawnExtensionComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Shared")
	void RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWeaponBase* Weapon, bool bRegisterAsEquippedWeapon = false);
	
	UFUNCTION(BlueprintCallable, Category = "Hero")
	AWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Hero")
	FGameplayTag GetCurrentEquipTag();
	
	UPROPERTY(BlueprintReadWrite, Category = "Hero")
	FGameplayTag CurrentEquippedWeaponTag;
	
	UFUNCTION(BlueprintCallable, Category = "Hero")
	AWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleWeaponCollision(bool bUse, EToggleDamageType ToggleDamagetype = EToggleDamageType::CurrentEquippedWeapon);

	UFUNCTION(BlueprintCallable, Category = "Shared")
	TMap<FGameplayTag, AWeaponBase*> GetWeaponMap() { return CharacterCarriedWeaponMap;};

	UFUNCTION(BlueprintCallable, Category = "Shared")
	virtual void SetParryable(bool bNewParryable);

	//Hitreact
	virtual void OnHitTargetActor(AActor* HitActor);
	
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
	TArray<AActor*> OverlappedActors;
	bool bIsParryable;
	
	virtual void ToggleCurrentEquippedWeaponCollision(bool bUse);
	virtual void ToggleBodyCollision(bool bUse,EToggleDamageType ToggleDamageType);
private:
	TMap<FGameplayTag, AWeaponBase*> CharacterCarriedWeaponMap;
	
};
