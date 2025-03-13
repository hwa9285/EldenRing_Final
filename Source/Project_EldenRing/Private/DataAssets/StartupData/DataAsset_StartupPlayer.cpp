// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartupData/DataAsset_StartupPlayer.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseType/BaseStruct.h"

void UDataAsset_StartupPlayer::GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level)
{
	Super::GiveToAbilitySystemComponent(ASC, Level);

	for(const FPlayerAbilitySet& AbilitySet : PlayerStartupAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;
    
		FGameplayAbilitySpec Spec(AbilitySet.AbilityToGrant);
		Spec.SourceObject = ASC->GetAvatarActor();
		Spec.Level = Level;

		Spec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		ASC->GiveAbility(Spec);
	}
}
