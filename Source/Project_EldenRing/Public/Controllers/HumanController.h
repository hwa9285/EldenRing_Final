// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "HumanController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

class UWidgetBase;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API AHumanController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AHumanController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UWidgetBase> PlayerHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UWidgetBase> PlayerMenuWidget;
	
	virtual FGenericTeamId GetGenericTeamId() const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Player")
	APlayerCharacter* PlayerChar;

	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerChr(){return PlayerChar;};
	

	UPROPERTY(BlueprintAssignable, Category = "Pawn")
	FOnPawnChanged OnPawnChanged;
	
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	FGenericTeamId PlayerTeamId;

};
