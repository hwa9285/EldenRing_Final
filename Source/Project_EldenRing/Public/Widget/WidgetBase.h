// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

class UHumanCombatComponent;
class UInventoryManagerComponent;
class UStatComponent;
class UPlayerUIComponent;
class UEnemyUIComponent;

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* PlayerUIComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* EnemyUIComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnOwningPlayerStatComponentInitialized(UStatComponent* PlayerStatComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnOwningInventoryComponentInitialized(UInventoryManagerComponent* InventoryManagerComponent);

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreateWidget(AActor* EnemyActor);

	// 새 Pawn으로 재초기화하는 함수
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void ReinitializeBindings(APawn* NewPawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Owner")
	UWidgetBase* OwningWidget;
	
};
