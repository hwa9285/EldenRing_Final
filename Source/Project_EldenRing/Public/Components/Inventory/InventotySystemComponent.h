// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventotySystemComponent.generated.h"


class UEnhancedInputLocalPlayerSubsystem;
class UInteractTextWidget;
class UShowCurrentInteractItem;
class UInputMappingContext;
class UInventoryManagerComponent;
class APlayerCharacter;
class AItemBase;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_ELDENRING_API UInventotySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventotySystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	APlayerCharacter* PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    UInventoryManagerComponent* PlayerInventoryManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInputMappingContext* IMC_Interact;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInputMappingContext* IMC_Interact_Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInputMappingContext* IMC_Interact_SavePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInputMappingContext* IMC_UI;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	//UInputMappingContext* IMC_BackKey;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInteractTextWidget* InteractText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UShowCurrentInteractItem* CurrentInteractItemWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	APlayerController* PC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    UEnhancedInputLocalPlayerSubsystem* InputSubsystem;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool FlipFlopMenuUI = false;
    bool FlipFlopInteractItem = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool FlipFlopInteractSavepoint = false;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	//FOnSetInteractName SetInteractName;
	
	UFUNCTION(BlueprintCallable)
	void StartUp();
	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							  const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ToggleMenuUI();

	UFUNCTION(BlueprintCallable)
	void ToggleSavepoint();
	
	UFUNCTION(BlueprintCallable)
	void ToggleHUD(bool ToggleBool);


	
	UFUNCTION(BlueprintCallable)
	void InteractItem();

	UFUNCTION(BlueprintCallable)
	void InteractTalk();
};
