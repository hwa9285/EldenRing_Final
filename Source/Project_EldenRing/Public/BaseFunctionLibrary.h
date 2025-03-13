// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseType/BaseEnum.h"
#include "BaseFunctionLibrary.generated.h"

struct FGameplayTag;
class UBaseAbilitySystemComponent;
class UPawnCombatComponent;

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UBaseFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UBaseAbilitySystemComponent* NativeGetAbilitySystemComponentFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void AddGameplayTagToActor(AActor* Actor, const FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void RemoveGameplayTagFromActor(AActor* Actor, const FGameplayTag Tag);

	static bool NativeActorHasTag(AActor* Actor, const FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta=(Display="Does Actor Has Tag", ExpandEnumAsExecs = "OutType"))
	static void BP_HasTag(AActor* Actor, const FGameplayTag Tag, EBaseConfirmType& OutType);
	
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (Display = "Get PawnCombatComponent From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* Actor, EBaseValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static bool IsTargetPawnHostile(const APawn* OwningPawn,const APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* Attacker, AActor* HitActor, float& OutAngleDifference);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static EDirectionType FindDirectionTargetActor(AActor* TargetActor, AActor* SelfActor);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static bool IsValidBlock(const AActor* Attacker, const AActor* Defender);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static bool IsValidFront(const AActor* Attacker, const AActor* Defender);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* Instigator, AActor* TargetActor, const FGameplayEffectSpecHandle& OutSpecHandle);
};
