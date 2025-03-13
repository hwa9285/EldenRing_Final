// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartupData/DataAsset_StartupBase.h"
#include "DataAsset_StartupHorse.generated.h"

struct FPlayerAbilitySet;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UDataAsset_StartupHorse : public UDataAsset_StartupBase
{
	GENERATED_BODY()

protected:
	virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level = 1) override;
	 
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartupData", meta=(TitleProPerty="InputTag"))
	TArray<FPlayerAbilitySet> PlayerStartupAbilitySets;
};
