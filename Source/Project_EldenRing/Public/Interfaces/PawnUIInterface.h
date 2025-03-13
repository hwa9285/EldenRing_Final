// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnUIInterface.generated.h"


class UHumanCombatComponent;
class UInventoryManagerComponent;
class UStatComponent;
class UEnemyUIComponent;
class UPawnUIComponent;
class UPlayerUIComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_ELDENRING_API IPawnUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UPawnUIComponent* GetPawnUIComponent() const = 0;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const;
	virtual UEnemyUIComponent* GetEnemyUIComponent()const;
	virtual UStatComponent* GetStatComponent()const;
	virtual UInventoryManagerComponent* GetInventoryManagerComponent()const;
};
