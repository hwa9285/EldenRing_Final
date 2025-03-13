// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemPickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemPickupInterface : public UInterface
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FItemInfos //: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemTypeID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity = 1;
};

UCLASS(BlueprintType)
class PROJECT_ELDENRING_API UItemInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FItemInfos ItemInfo;
};

USTRUCT(BlueprintType)
struct FItemInfoArray
{
	GENERATED_BODY()
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	//TArray<FItemInfos> TypeItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TArray<TObjectPtr<UItemInfo>> TypeItemsPtr;

	
	FItemInfoArray()
	{
	}
};
/**
 * 
 */








class PROJECT_ELDENRING_API IItemPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// UFUNCTION()
	// virtual void PickupItem(AActor* IntercatActor);
	
	// 이름 반환 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	FName GetName() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	FItemInfos GetItemInfos() const;
};
