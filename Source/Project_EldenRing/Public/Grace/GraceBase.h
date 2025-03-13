// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GraceInterface.h"
#include "GraceBase.generated.h"

UCLASS()
class PROJECT_ELDENRING_API AGraceBase : public AActor, public IGraceInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraceBase();

	virtual void SaveData_Implementation(APlayerCharacter* PlayerCharacter) override;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void InvigorationGrace();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void EndInvigorationGrace();

};
