// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/EnemyUIComponent.h"
#include "Widget/WidgetBase.h"

void UEnemyUIComponent::RegisterEnemyWidget(UWidgetBase* Widget)
{
	EnemyWidgets.Add((Widget));
}

void UEnemyUIComponent::RemoveEnemyWidget()
{
	if (EnemyWidgets.IsEmpty())
	{
		return;
	}

	for (UWidgetBase* Widget : EnemyWidgets)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();
		}
	}

	EnemyWidgets.Empty();
}
