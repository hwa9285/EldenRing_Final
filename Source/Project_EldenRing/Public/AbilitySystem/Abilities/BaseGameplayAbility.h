// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"


class UPawnCombatComponent;
class UBaseAbilitySystemComponent;

UENUM(BlueprintType)
enum class EBaseAbilityActivationPolcy : uint8
{
	OnTriggered,
	OnGiven
};

USTRUCT(BlueprintType)
struct FAttributeLevels
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellExecutionData")
	uint8 BleedLev;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellExecutionData")
	uint8 BurnLev;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellExecutionData")
	uint8 DebuffAtkLev;
};

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	
	UPROPERTY(EditDefaultsOnly, Category = "BaseAbility")
	EBaseAbilityActivationPolcy AbilityActivationPolicy = EBaseAbilityActivationPolcy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& SpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Ability", meta=(Display="Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& SpecHandle, EBaseSuccessType& OutSuccessType);

	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayEffectSpecHandle MakeAttributeEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect,
	const FScalableFloat& BleedAmount,const FScalableFloat& BurnAmount ,const FScalableFloat& DebuffAtkAmount,const FAttributeLevels AttLevels);
	
	
	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayEffectSpecHandle MakeTickDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect,const float BaseDamage,EAttributeType Type);

	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ClearAllLooseTag(FGameplayTag Tag);
	
	
};
