// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

class USlider;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateProgressBar(float Progress);

protected:
	UPROPERTY(meta = (BindWidget))
	USlider* LoadingProgressBar;
};
