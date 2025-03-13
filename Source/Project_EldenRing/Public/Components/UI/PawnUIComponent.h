// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponent.h"
#include "PawnUIComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangeDelegate,float,NewPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTakenDelegate,int32,NewDamage);
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPawnUIComponent : public UPawnExtensionComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnPercentChangeDelegate OnCurrentHPChanged;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnDamageTakenDelegate OnDamageTakenChanged;
};
