// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_PlayMonatageCustom.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UAbilityTask_PlayMonatageCustom : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Montage", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_PlayMonatageCustom* PlayMontageAndWaitCustom(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* InMontageToPlay,
		float InRate = 1.f,
		FName InStartSection = NAME_None,
		bool InStopWhenAbilityEnds = true,
		float InAnimRootMotionTranslationScale = 1.f,
		USkeletalMeshComponent* InCustomSkeletalMesh = nullptr);

	/** 사용할 커스텀 SkeletalMeshComponent */
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Montage")
	USkeletalMeshComponent* CustomSkeletalMesh;

	
protected:
	// Activate()를 오버라이드해서 CustomSkeletalMesh가 있다면 이를 사용
	virtual void Activate() override;
	
	// 몽타주 재생 시 반환되는 재생 인스턴스의 ID 또는 상태를 저장합니다.
	float MontageInstance;
};
