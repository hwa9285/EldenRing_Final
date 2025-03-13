// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/PlayerAnimInstance.h"
#include "HumanPlayerAnimInstance.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UHumanPlayerAnimInstance : public UPlayerAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
	APlayerCharacter* OwningPlayerCharacter;

	
};
