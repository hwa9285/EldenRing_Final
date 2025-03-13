// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "PlayerLinkedAnimLayer.generated.h"

class UHorseHumanAnimInstance;
class UHumanPlayerAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPlayerLinkedAnimLayer : public UBaseAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta= (BlueprintThreadSafe))
	UHumanPlayerAnimInstance* GetPlayerAnimInstance() const;

	UFUNCTION(BlueprintPure, meta= (BlueprintThreadSafe))
	UHorseHumanAnimInstance* GetHorseAnimInstance() const;
};
