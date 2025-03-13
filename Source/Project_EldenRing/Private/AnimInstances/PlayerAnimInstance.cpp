// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/PlayerAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	//소유한 캐릭터의 속도
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	Speed = OwningCharacter->GetVelocity().Size();
	Dir = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());

	bIsFalling = OwningMovementComponent->IsFalling();
	

	//소유한 캐릭터무브먼트의 움직임
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
