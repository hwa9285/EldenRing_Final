// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InventoryWidget/InteractTextWidget.h"

void UInteractTextWidget::UpdateText(const FText& NewText)
{
	InteractText->SetText(NewText);
}
