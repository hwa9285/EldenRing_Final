// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/EnemyAttributeSet.h"

#include "BaseFunctionLibrary.h"
#include "GameplayEffectExtension.h"

UEnemyAttributeSet::UEnemyAttributeSet()
{
}

void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float NewCurrentHp = FMath::Clamp(GetCurrentHp(),0.0f,GetMaxHp());

		if (NewCurrentHp * 2.0f <= GetMaxHp())
		{
			UBaseFunctionLibrary::AddGameplayTagToActor(Data.Target.GetAvatarActor(), BaseGamePlayTags::Enemy_Status_Phase2);
		}
	}
}
