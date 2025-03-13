// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ItemSpawnBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API AItemSpawnBase : public AItemBase
{
	GENERATED_BODY()
public:
	AItemSpawnBase();
public:
	
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* BaseMeshComponent;
};
