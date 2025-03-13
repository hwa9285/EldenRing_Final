// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StatSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UStatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UStatSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	class UStatManager* StatManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	UCurveTable* StatCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	UCurveTable* CostRuneTable;
	
};
