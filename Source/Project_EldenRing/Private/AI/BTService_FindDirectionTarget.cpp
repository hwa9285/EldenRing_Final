// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindDirectionTarget.h"
#include "AbilitySystem/Abilities/EnemyGameplayAbility.h"

void UBTService_FindDirectionTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//UEnemyGameplayAbility::FindDirectionTargetActor(TargetKey->,OwnerKey);
}
