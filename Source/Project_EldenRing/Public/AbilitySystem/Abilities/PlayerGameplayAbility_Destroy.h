// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"
#include "BaseType/BaseStruct.h"
#include "PlayerGameplayAbility_Destroy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPlayerGameplayAbility_Destroy : public UPlayerGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void DestroyWeapon();
	
private:
	ESpawnWeaponType CurrentType;
	
	int32 Slot;
};
