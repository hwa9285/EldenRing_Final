// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventotySystemComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Character/PlayerCharacter.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/InventoryWidget/InteractTextWidget.h"
#include "Widget/InventoryWidget/ShowCurrentInteractItem.h"

// Sets default values for this component's properties
UInventotySystemComponent::UInventotySystemComponent()
{

}


// Called when the game starts
void UInventotySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventotySystemComponent::StartUp()
{
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerInventoryManager = PlayerCharacter->GetInventoryManager();
	PC = Cast<APlayerController>(PlayerCharacter->GetController());
	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	
	FItemInfos RedPotion;
	RedPotion.ItemID = "100";
	RedPotion.ItemTypeID = "1";
	RedPotion.Quantity = 3;
	PlayerInventoryManager->AddToInventoryWithItemInfo(RedPotion);

	FItemInfos ManaPotion;
	ManaPotion.ItemID = "101";
	ManaPotion.ItemTypeID = "1";
	ManaPotion.Quantity = 3;
	PlayerInventoryManager->AddToInventoryWithItemInfo(ManaPotion);
	
	FItemInfos Horse;
	Horse.ItemID = "102";
	Horse.ItemTypeID = "1";
	Horse.Quantity = 1;
	PlayerInventoryManager->AddToInventoryWithItemInfo(Horse);
	
	FItemInfos FirstSword;
	FirstSword.ItemID = "1";
	FirstSword.ItemTypeID = "1";
	FirstSword.Quantity = 1;
	PlayerInventoryManager->AddToInventoryWithItemInfo(FirstSword);

	// PlayerInventoryManager->OnComponentBeginOverlap.AddDynamic(this, &UInventotySystemComponent::OnSphereBeginOverlap);
	// PlayerInventoryManager->OnComponentEndOverlap.AddDynamic(this, &UInventotySystemComponent::OnSphereEndOverlap);
}

void UInventotySystemComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

			InputSubsystem->RemoveMappingContext(IMC_Interact);
			InputSubsystem->RemoveMappingContext(IMC_Interact_Item);
			InputSubsystem->RemoveMappingContext(IMC_Interact_SavePoint);
	
			InteractText->SetVisibility(ESlateVisibility::Visible);
			FString InteractString;
			
			//PlayerInventoryManager->SelectInteractType
			switch (PlayerInventoryManager->SelectInteractType)
			{
			case EInteractType::Talk:
				InputSubsystem->AddMappingContext(IMC_Interact, 2);
				//InteractText->UpdateText(FText::FromName(": 아이템을 줍는다."));
				InteractString = TEXT(" : 말을 건다.");
				break;
			case EInteractType::SavePoint:
				InputSubsystem->AddMappingContext(IMC_Interact_SavePoint, 2);
				InteractString = TEXT(" : 축복에서 쉰다.");;
				break;
			case EInteractType::PickUpItem:
				InputSubsystem->AddMappingContext(IMC_Interact_Item, 2);
				InteractString = TEXT(": 아이템을 줍는다.");;
				break;
			}
			PlayerInventoryManager->OnSetInteractName.Broadcast(InteractString);
			PlayerInventoryManager->SelectInteractType = EInteractType::None;

}

void UInventotySystemComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// InputSubsystem->RemoveMappingContext(IMC_Interact);
	// InputSubsystem->RemoveMappingContext(IMC_Interact_Item);
	// InputSubsystem->RemoveMappingContext(IMC_Interact_SavePoint);

	PlayerInventoryManager->CurrentInteractItem = nullptr;
	InteractText->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventotySystemComponent::ToggleMenuUI()
{
	if (!FlipFlopMenuUI)
	{
		FlipFlopMenuUI = true;

		PC->SetInputMode(FInputModeGameAndUI());
		PC->bShowMouseCursor = true;
		PlayerInventoryManager->MyWidgetSwitcherReference->SetVisibility(ESlateVisibility::Visible);
		PlayerInventoryManager->MyWidgetSwitcherReference->SetActiveWidgetIndex(0);
		PC->SetViewTargetWithBlend(PlayerCharacter, 0.0f, EViewTargetBlendFunction::VTBlend_Cubic, 0.0f, false);
		PlayerCharacter->GetCharacterMovement()->DisableMovement();

	}
	else
	{
		FlipFlopMenuUI = false;

		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		PlayerInventoryManager->MyWidgetSwitcherReference->SetVisibility(ESlateVisibility::Collapsed);
		PlayerInventoryManager->MyWidgetSwitcherReference->SetActiveWidgetIndex(0);
		PC->SetViewTargetWithBlend(PlayerCharacter, 0.0f, EViewTargetBlendFunction::VTBlend_Cubic, 0.0f, false);
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void UInventotySystemComponent::ToggleSavepoint()
{
	ToggleHUD(FlipFlopInteractSavepoint);
}

void UInventotySystemComponent::ToggleHUD(bool ToggleBool)
{
	if (!ToggleBool)
	{
		ToggleBool = true;

		PC->SetInputMode(FInputModeGameAndUI());
		PC->bShowMouseCursor = true;
		InteractText->SetVisibility(ESlateVisibility::Collapsed);
		PlayerInventoryManager->MyWidgetSwitcherReference->SetVisibility(ESlateVisibility::Visible);
		PlayerInventoryManager->MyWidgetSwitcherReference->SetActiveWidgetIndex(3);
		PC->SetViewTargetWithBlend(PlayerCharacter, 0.0f, EViewTargetBlendFunction::VTBlend_Cubic, 0.0f, false);
		PlayerCharacter->GetCharacterMovement()->DisableMovement();
		InputSubsystem->AddMappingContext(IMC_UI, 3);
	}
	else
	{
		ToggleBool = false;

		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		PlayerInventoryManager->MyWidgetSwitcherReference->SetVisibility(ESlateVisibility::Collapsed);
		PlayerInventoryManager->MyWidgetSwitcherReference->SetActiveWidgetIndex(0);
		PC->SetViewTargetWithBlend(PlayerCharacter, 0.0f, EViewTargetBlendFunction::VTBlend_Cubic, 0.0f, false);
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		InputSubsystem->RemoveMappingContext(IMC_UI);
	}
}

void UInventotySystemComponent::InteractItem()
{
	// if (PlayerInventoryManager->CurrentInteractItem)
	// {
	// 	if (!FlipFlopInteractItem)
	// 	{
	// 		FlipFlopInteractItem = true;
	// 		InteractText->SetVisibility(ESlateVisibility::Collapsed);
	// 		CurrentInteractItemWidget->UpdateImageText(PlayerInventoryManager->ItemName);
	// 		CurrentInteractItemWidget->SetVisibility(ESlateVisibility::Visible);
	// 	}
	// 	else
	// 	{
	// 		FlipFlopInteractItem = false;
	// 		CurrentInteractItemWidget->SetVisibility(ESlateVisibility::Collapsed);
	// 		InputSubsystem->RemoveMappingContext(IMC_Interact_Item);
	// 		PlayerInventoryManager->AddToInventory();
	// 	}
	// }
}

void UInventotySystemComponent::InteractTalk()
{
	InteractText->SetVisibility(ESlateVisibility::Collapsed);
	FName ItemID = PlayerInventoryManager->ItemID;
}



