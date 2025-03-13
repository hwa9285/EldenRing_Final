// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Horse.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AHorse::AHorse()
{
	//회전 사용 비활성화
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	//케릭터움직임 초기세팅
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	//매시 세팅
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Assets/AdamHorsemanKnight/Character/Horse/SM_KnightHorse.SM_KnightHorse"));

	if (MeshAsset.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.05f), FRotator(0, -90.0f, 0));

	TeamId = FGenericTeamId(0);
}

FGenericTeamId AHorse::GetGenericTeamId() const
{
	return TeamId;
}

void AHorse::AssignGrantedHorseAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
{
	AbilitySpecHandles = SpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AHorse::GetGrantedHorseAbilitySpecHandles() const
{
	return AbilitySpecHandles;
}

