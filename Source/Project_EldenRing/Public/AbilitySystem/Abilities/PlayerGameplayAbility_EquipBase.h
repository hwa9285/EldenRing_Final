// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PaperSprite.h"
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"
#include "BaseType/BaseEnum.h"
#include "PlayerGameplayAbility_EquipBase.generated.h"


class IPawnUIInterface;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPlayerGameplayAbility_EquipBase : public UPlayerGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:	
	TWeakInterfacePtr<IPawnUIInterface> GetPawnUIInterface(){return CachedUIInterface;};

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedUIInterface;

	void EquipSpell();
};
