// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable, Category = "RespawnPlayer")
	//virtual void RestartPlayer(AController* NewPlayer) override;
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetPlayerMesh();
	
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(AController* NewPlayer);
};
