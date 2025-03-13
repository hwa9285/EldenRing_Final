// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/HorseHumanAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/BaseCharacter.h"
#include "Character/Horse.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHorseHumanAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (OwningCharacter)
	{
		OwningHorse = Cast<AHorse>(OwningCharacter);
		checkf(OwningHorse,TEXT("Horse Is Null"));
		OwningMovementComponent = OwningHorse ->GetCharacterMovement();
		
	}
}

void UHorseHumanAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}
	if (OwningHorse && OwningHorse->GetPlayer())
	{
		RidingCharacter = OwningHorse->GetPlayer();
		// 필요하다면 여기서 RidingCharacter의 이동 컴포넌트 등 추가 정보를 업데이트합니다.
		PlayerMovementComponent = RidingCharacter->GetCharacterMovement();
	}
	if (!RidingCharacter || !PlayerMovementComponent)
	{
		return;
	}

	//소유한 캐릭터의 속도
	//GroundSpeed = OwningCharacter->GetVelsocity().Size2D();

	// Speed = RidingCharacter->GetVelocity().Size();
	//
	//
	// float temp = FVector::DotProduct(RidingCharacter->GetVelocity(),RidingCharacter->GetActorForwardVector());
	// bool Condition = SpeedForward >= temp;
	// SpeedForward = FMath::Lerp(SpeedForward, temp,Condition ? 0.1f : 0.5f );
	//
	// bIsFalling = PlayerMovementComponent->IsFalling();
	// SpeedSide = UKismetAnimationLibrary::CalculateDirection(RidingCharacter->GetVelocity(), RidingCharacter->GetActorRotation());
	//
	 Speed = OwningHorse->GetVelocity().Size();
	
	
	 float temp = FVector::DotProduct(OwningHorse->GetVelocity(),OwningHorse->GetActorForwardVector());
	 bool Condition = SpeedForward >= temp;
	 SpeedForward = FMath::Lerp(SpeedForward, temp,Condition ? 0.1f : 0.5f );
	
	 bIsFalling = OwningMovementComponent->IsFalling();
	 SpeedSide = UKismetAnimationLibrary::CalculateDirection(OwningHorse->GetVelocity(), OwningHorse->GetActorRotation());
	 SpeedSide = FMath::Lerp(SpeedSide,FVector::DotProduct(OwningHorse->GetVelocity(),OwningHorse->GetActorRightVector()),0.2f);

}
