// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Human/PlayerLinkedAnimLayer.h"

#include "AnimInstances/HorseHumanAnimInstance.h"
#include "AnimInstances/Human/HumanPlayerAnimInstance.h"
#include "Character/Horse.h"
#include "Character/PlayerCharacter.h"

UHumanPlayerAnimInstance* UPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UHumanPlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}

UHorseHumanAnimInstance* UPlayerLinkedAnimLayer::GetHorseAnimInstance() const
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningComponent()->GetOwner());
	if (PlayerCharacter)
	{
		return Cast<UHorseHumanAnimInstance>(PlayerCharacter->GetHorse()->GetMesh()->GetAnimInstance());
	}
	return nullptr;
}
