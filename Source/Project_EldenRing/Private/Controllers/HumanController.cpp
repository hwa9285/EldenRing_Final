// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HumanController.h"

#include "Blueprint/UserWidget.h"
#include "Character/PlayerCharacter.h"
#include "Widget/WidgetBase.h"

AHumanController::AHumanController()
{
	PlayerTeamId = FGenericTeamId(0);
}


FGenericTeamId AHumanController::GetGenericTeamId() const
{
	return PlayerTeamId;
}

void AHumanController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// PlayerChar = Cast<APlayerCharacter>(InPawn);
 //    
	// // Possess 되었을 때 OnPawnChanged 델리게이트 호출
	// OnPawnChanged.Broadcast(InPawn);
	//
	// // 이미 위젯이 생성되어 있다면 새로 생성하지 않고 바인딩만 재설정
	// if (PlayerChar)
	// {
	// 	if (PlayerChar->PlayerHud)
	// 	{
	// 		PlayerChar->PlayerHud->ReinitializeBindings(InPawn);
	// 	}
	// 	if (PlayerChar->MenuWidget)
	// 	{
	// 		PlayerChar->MenuWidget->ReinitializeBindings(InPawn);
	// 	}
	// }

	
	PlayerChar = Cast<APlayerCharacter>(InPawn);
	OnPawnChanged.Broadcast(InPawn);
    
	// 위젯 생성 및 할당 (이미 생성된 위젯이 있다면 재초기화)
	if (PlayerChar)
	{
		if (!PlayerChar->PlayerHud && PlayerHUDWidget)
		{
			if (UWidgetBase* HUD = CreateWidget<UWidgetBase>(this, PlayerHUDWidget))
			{
				HUD->AddToViewport();
				PlayerChar->PlayerHud = HUD;
				OnPawnChanged.AddDynamic(HUD, &UWidgetBase::ReinitializeBindings);
			}
		}
		if (!PlayerChar->MenuWidget && PlayerMenuWidget)
		{
			if (UWidgetBase* Menu = CreateWidget<UWidgetBase>(this, PlayerMenuWidget))
			{
				Menu->AddToViewport();
				PlayerChar->MenuWidget = Menu;
				Menu->SetVisibility(ESlateVisibility::Collapsed);
				OnPawnChanged.AddDynamic(Menu, &UWidgetBase::ReinitializeBindings);
			}
		}
	}
}

