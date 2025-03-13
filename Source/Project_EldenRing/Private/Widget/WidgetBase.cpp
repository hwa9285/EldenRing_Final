// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetBase.h"

#include "Components/Combat/HumanCombatComponent.h"
#include "Components/Stat/StatComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

void UWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APawn* OwningPawn = GetOwningPlayerPawn();
	ReinitializeBindings(OwningPawn);
}

void UWidgetBase::ReinitializeBindings(APawn* NewPawn)
{
	//새 폰으로 possess되면 실행
	if (!NewPawn) return;

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(NewPawn))
	{
		if (UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent())
		{
			BP_OnOwningPlayerUIComponentInitialized(PlayerUIComponent);
		}

		if (UStatComponent* StatComponent = PawnUIInterface->GetStatComponent())
		{
			BP_OnOwningPlayerStatComponentInitialized(StatComponent);
		}

		if (UInventoryManagerComponent* InventoryComponent = PawnUIInterface->GetInventoryManagerComponent())
		{
			BP_OnOwningInventoryComponentInitialized(InventoryComponent);
		}
	}
}

void UWidgetBase::InitEnemyCreateWidget(AActor* EnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(EnemyActor))
	{
		if (UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent())
		{
			BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
		}
	}
}
