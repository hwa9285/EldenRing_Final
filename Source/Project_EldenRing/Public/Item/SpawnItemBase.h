// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ItemPickupInterface.h"
#include "SpawnItemBase.generated.h"

UCLASS()
class PROJECT_ELDENRING_API ASpawnItemBase : public AActor, public IItemPickupInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnItemBase();

protected:


public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemName")
	FName ItemDTName;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* ItemMeshComponent;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	// USkeletalMeshComponent* EquipmentItemMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* SceneComponent;


	// 인터페이스 함수 구현
	virtual FName GetName_Implementation() const override;

};
