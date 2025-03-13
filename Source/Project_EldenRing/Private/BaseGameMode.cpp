// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

#include "SubSystem/SaveSubsystem.h"

void ABaseGameMode::RespawnPlayer(AController* NewPlayer)
{
	if (!NewPlayer) return;

	USaveSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveSubsystem>();

	// 저장된 리스폰 위치.
	FTransform Respawn = Subsystem->RespawnTransform;
	
	APawn* NewPawn = SpawnDefaultPawnAtTransform(NewPlayer, Respawn);
	if (NewPawn)
	{
		NewPlayer->Possess(NewPawn);
	}

	//AHumanController* Controller = NewPlayer;
}
