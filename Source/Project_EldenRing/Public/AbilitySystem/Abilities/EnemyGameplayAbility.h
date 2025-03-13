// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseType/BaseEnum.h"

#include "EnemyGameplayAbility.generated.h"

class AEnemyCharacter;
class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UEnemyGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AbilitySystem")
	AEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "AbilitySystem")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	//UFUNCTION(BlueprintPure, Category = "Ability")
	//FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect, const FScalableFloat& DamageFloat , const float ImpulseBase);

	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageAndImpulseEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect, const FScalableFloat& DamageFloat , const FScalableFloat& ImpulseBase);
	
private:
	TWeakObjectPtr<AEnemyCharacter> CachedEnemyCharacter;
};
