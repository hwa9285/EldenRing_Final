// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NpcInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNpcInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_ELDENRING_API INpcInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractNPC")
	void InteractNPC(APlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractNPC")
	void SetNPCScript(AActor* SendActor,const TArray<FText>& TextArray);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractNPC")
	void SetNPC(AActor* SendActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractNPC")
	void EndInteractNPC();
};
