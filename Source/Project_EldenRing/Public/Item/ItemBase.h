// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ItemPickupInterface.h"
#include "ItemBase.generated.h"

class UDataTable;

UCLASS(Blueprintable)
class PROJECT_ELDENRING_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

public:	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* ItemMeshComponent;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	// USkeletalMeshComponent* EquipmentItemMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* SceneComponent;
};
