// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ItemPickupInterface.h"
#include "DroppedItemBase.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT_ELDENRING_API ADroppedItemBase : public AActor, public IItemPickupInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroppedItemBase();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UBoxComponent* ItemCollsionBox;
private:

public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DroppedItem",meta=(AllowPrivateAccess=true))
	FName DroppedItemName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DroppedItem",meta=(AllowPrivateAccess=true))
	FName WeaponStatisticsName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DroppedItem",meta=(AllowPrivateAccess=true))
	int32 DroppedItemAmount;
	
	// 인터페이스 함수 구현
	virtual FName GetName_Implementation() const override;
	virtual FItemInfos GetItemInfos_Implementation() const override;
};
