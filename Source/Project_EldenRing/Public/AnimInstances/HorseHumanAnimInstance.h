// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/PlayerAnimInstance.h"
#include "HorseHumanAnimInstance.generated.h"

class APlayerCharacter;
//class APlayerCharacter;
class AHorse;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UHorseHumanAnimInstance : public UPlayerAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override; //초기값 설정된
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override; //업데이트시 최적화 시킬수 있는 함수

protected:
	UPROPERTY()
	AHorse* OwningHorse;

	UPROPERTY()
	APlayerCharacter* RidingCharacter;

	UPROPERTY()
	UCharacterMovementComponent* PlayerMovementComponent;
};
