// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartupData/DataAsset_StartupBase.h"
#include "DataAsset_StartupEnemy.generated.h"

class UEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UDataAsset_StartupEnemy : public UDataAsset_StartupBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level = 1) override;

private:
	UPROPERTY(EditDefaultsOnly,Category="StartupData")
	TArray<TSubclassOf<UEnemyGameplayAbility>> EnemyCombatAbilities;
};
