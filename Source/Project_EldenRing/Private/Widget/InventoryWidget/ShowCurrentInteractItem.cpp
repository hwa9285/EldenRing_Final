// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InventoryWidget/ShowCurrentInteractItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Item/Subsystem/InventoryGameInstanceSubsystem.h"


void UShowCurrentInteractItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UInventoryManagerComponent* InventoryComponent = PawnUIInterface->GetInventoryManagerComponent())
		{
			InventoryComponent->OnGetInteractName.BindUFunction(this, FName("UpdateImageText"));
			//.BindUFunction(this, FName("HandleInteract"));
		}
	}
}

void UShowCurrentInteractItem::UpdateImageText(FName ItemName)
{
    //SetVisibility(ESlateVisibility::Visible);
	
	if (ItemName.IsValid())
	{
		UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();

		FText ItemText = FText::FromName(Subsystem->GetDroppedItemData(ItemName).ItemName);
		CurrentItemText->SetText(ItemText);

		Subsystem->GetDroppedItemData(ItemName).ItemIcon;
		CurrentItemImage->SetBrushFromAtlasInterface(Subsystem->GetDroppedItemData(ItemName).ItemIcon);
	}
}
