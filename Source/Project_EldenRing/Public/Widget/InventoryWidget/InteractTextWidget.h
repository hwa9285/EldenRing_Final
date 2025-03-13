// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Widget/WidgetBase.h"
#include "InteractTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UInteractTextWidget : public UWidgetBase
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractText;

	UFUNCTION(BlueprintCallable)
	void UpdateText(const FText& NewText);
};
