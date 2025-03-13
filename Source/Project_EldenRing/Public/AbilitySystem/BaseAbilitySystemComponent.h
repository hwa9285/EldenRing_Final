// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseType/BaseStruct.h"
#include "GameplayTagContainer.h"
#include "BaseAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	void OnAbilityInputReleased(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category="Ability", meta=(Level="1"))
	void GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitySet>& WeaponAbilities, int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category="Ability", meta=(Level="1"))
	void GrantPlayerAshOfWarAbility(const FPlayerAbilitySet& AshOfWarAbility,int32 Level, FGameplayAbilitySpecHandle& OutGrantedAbilitySpecHandle);

	UFUNCTION(BlueprintCallable, Category="Ability", meta=(Level="1"))
	void GrantHorseAbility(const TArray<FPlayerAbilitySet>& HorseAbilitieses,int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandle);
	
	UFUNCTION(BlueprintCallable, Category="Ability")
	void RemoveGrantedAshOfWatAbility(UPARAM(ref) FGameplayAbilitySpecHandle& SpecHandleToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveGrantedHorseAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool HasAbilityOfClass(TSubclassOf<UGameplayAbility> AbilityClass)const;
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryActivateAbilityByTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void CancelAbilityByTag(FGameplayTag Tag);
};
