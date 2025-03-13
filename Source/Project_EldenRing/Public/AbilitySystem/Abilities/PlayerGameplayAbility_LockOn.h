// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"
#include "PlayerGameplayAbility_LockOn.generated.h"

class UInputMappingContext;
class UWidgetBase;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPlayerGameplayAbility_LockOn : public UPlayerGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();

	UFUNCTION(BlueprintCallable)
	void OnTargetingTick(float DeltaTime);
	
	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float TraceDistance = 2500.0f;

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	FVector TraceBoxSize = FVector(2500.0f, 2500.0f, 300.f);
	
	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceChannel;

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	bool bShowDebugShape;

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	TArray<AActor*> AvailableActorsToLock;

	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& AvailableActors);
	void CancelTargetingAbility();
	void CleanUp();

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	AActor* CurrentTarget;

	void DrawTargetingWidget();

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	TSubclassOf<UWidgetBase> TargetingWidgetClass;

	UPROPERTY()
	UWidgetBase* TargetingWidget;

	void SetTargetingWidgetPosition();

	UPROPERTY()
	FVector2D TargetingWidgetSize = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float TargetingCameraOffset = 20.0f;

	void InitTargetingMappingContext();
	void ResetTargetingMappingContext();

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	UInputMappingContext* TargetingMappingContext;
	
protected:
	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& SwitchDirectionTag);
	
	void GetAvailableActorAroundTarget(TArray<AActor*>& OutLeftActors, TArray<AActor*>& OutRightActors);
};
