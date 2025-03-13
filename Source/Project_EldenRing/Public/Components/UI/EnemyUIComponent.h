// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


class UWidgetBase;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyWidget(UWidgetBase* Widget);
	
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyWidget();

private:
	TArray<UWidgetBase*> EnemyWidgets;
};
