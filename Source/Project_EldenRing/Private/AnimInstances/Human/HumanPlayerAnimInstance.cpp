// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Human/HumanPlayerAnimInstance.h"

#include "Character/PlayerCharacter.h"

void UHumanPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningPlayerCharacter = Cast<APlayerCharacter>(OwningCharacter);
	}
}

void UHumanPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	
}
