// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Character/BaseCharacter.h"
#include "Horse.generated.h"

class APlayerCharacter;
struct FGameplayAbilitySpecHandle;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API AHorse : public ABaseCharacter , public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AHorse();

	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	FGenericTeamId TeamId;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	APlayerCharacter* PlayerCharacter;
	
public:
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayer(){return PlayerCharacter;}
	
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category="Horse")
	void HorseRiding(APlayerCharacter* Player);
	
	UFUNCTION(BlueprintCallable)
	void AssignGrantedHorseAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedHorseAbilitySpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};
