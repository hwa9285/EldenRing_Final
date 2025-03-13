// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/PawnUIInterface.h"
#include "ShowCurrentInteractItem.generated.h"

class UTextBlock;
class UImage;
class UInventoryManagerComponent;

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UShowCurrentInteractItem : public UUserWidget
{
	GENERATED_BODY()

public:

virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentItemText;

	UPROPERTY(meta = (BindWidget))
	UImage* CurrentItemImage;

	UFUNCTION(BlueprintCallable)
	void UpdateImageText(FName ItemName);


};
