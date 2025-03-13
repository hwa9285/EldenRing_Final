// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/PawnUIInterface.h"

// Add default functionality here for any IPawnUIInterface functions that are not pure virtual.

UPlayerUIComponent* IPawnUIInterface::GetPlayerUIComponent() const
{
	return nullptr;
}

UEnemyUIComponent* IPawnUIInterface::GetEnemyUIComponent() const
{
	return nullptr;
}

UStatComponent* IPawnUIInterface::GetStatComponent() const
{
	return nullptr;
}

UInventoryManagerComponent* IPawnUIInterface::GetInventoryManagerComponent() const
{
	return nullptr;
}
