// Fill out your copyright notice in the Description page of Project Settings.


#include "Grace/GraceBase.h"

#include "Character/PlayerCharacter.h"
#include "Components/Combat/HumanCombatComponent.h"
#include "SubSystem/SaveSubsystem.h"

// Sets default values
AGraceBase::AGraceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AGraceBase::SaveData_Implementation(APlayerCharacter* PlayerCharacter)
{
	IGraceInterface::SaveData_Implementation(PlayerCharacter);

	USaveSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveSubsystem>();
	Subsystem->SaveData(PlayerCharacter);

	PlayerCharacter->LoadAttribute(Subsystem->SaveAttributeData);
	PlayerCharacter->LoadInventory(Subsystem->SaveInventoryData);
	PlayerCharacter->GetHumanCombatComponent()->IsEnoughFP();
	
	InvigorationGrace();
	
	//UE_LOG(LogTemp, Warning, TEXT("DataSave Success"));
}


