// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindDirectionTarget.generated.h"

class UEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UBTService_FindDirectionTarget : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector OwnerKey;
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
