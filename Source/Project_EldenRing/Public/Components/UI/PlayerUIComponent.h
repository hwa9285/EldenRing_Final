// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/BaseEnum.h"
#include "Components/UI/PawnUIComponent.h"
#include "PlayerUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxAttributeChangeDelegate,float,NewMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckCanUseSkillDelegate,bool,NewBoolValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractActorDelegate,FString,Text,bool,IsInteracting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisibleWidget,bool,bIsVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCheckedSpellDelegate,bool,LeftWeapon,bool,RightWeapon,UPaperSprite*,Texture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddNewItemDelegate,FName,ItemName,UPaperSprite*,Icon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateItemDelegate,EConsumeType,Type,UPaperSprite*,Icon,uint8,Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateItemQuantityDelegate,FName,ID,uint8,NewQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateAttackAdditional,float,Newvalue,ESpawnWeaponType,Type);

/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UPlayerUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnUpdateAttackAdditional OnUpdateAttackAdditional;
	
	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnPercentChangeDelegate OnCurrentFPChanged;
	
	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnPercentChangeDelegate OnCurrentStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnMaxAttributeChangeDelegate OnMaxHpChanged;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnMaxAttributeChangeDelegate OnMaxFPChanged;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnMaxAttributeChangeDelegate OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnMaxAttributeChangeDelegate OnChangedRightHandAttack;
	
	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnMaxAttributeChangeDelegate OnChangedLeftHandAttack;

	UPROPERTY(BlueprintAssignable)
	FOnCheckedSpellDelegate OnCheckedSpell;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnCheckCanUseSkillDelegate OnCheckCanUseSkill;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnInteractActorDelegate OnInteractActor;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnVisibleWidget OnVisibleWidget;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnAddNewItemDelegate OnAddNewItem;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnAddNewItemDelegate OnAddNewItemByNPC;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnUpdateItemDelegate OnUpdateItem;

	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnUpdateItemQuantityDelegate OnUpdateItemQuantity;
};
