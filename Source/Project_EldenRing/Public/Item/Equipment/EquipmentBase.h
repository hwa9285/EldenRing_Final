// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "EquipmentBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT_ELDENRING_API AEquipmentBase : public AItemBase
{
	GENERATED_BODY()
	

public:
	AEquipmentBase();

protected:


public:
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	// UStaticMeshComponent* ItemMeshComponent;
};
