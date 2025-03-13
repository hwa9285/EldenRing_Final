// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BaseGamePlayTags.h"
#include "Character/EnemyCharacter.h"
#include "Components/BoxComponent.h"

#include "BaseDebugHelper.h"
#include "BaseFunctionLibrary.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	//Debug::Print(GetOwningPawn()->GetActorLabel() + TEXT(" is Hitting ") + HitActor->GetActorLabel());

	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.Add(HitActor);

	UE_LOG(LogTemp, Warning, TEXT("HitPawn: %s"),*HitActor->GetName());
	
	//TODO::Implement Block Check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UBaseFunctionLibrary::NativeActorHasTag(HitActor, BaseGamePlayTags::Player_Status_Block);;
	const bool bIsPlayerParring = UBaseFunctionLibrary::NativeActorHasTag(HitActor,BaseGamePlayTags::Shared_Status_Parry);
	const bool bIsPlayerUnbeatable = UBaseFunctionLibrary::NativeActorHasTag(HitActor,BaseGamePlayTags::Player_Status_Unbeatable);
	const bool bIsAttackUnBlockable = false;

	if ( bIsPlayerBlocking && !bIsAttackUnBlockable)
	{
		//Player block
		//플레이어가 정면에 있을 경우 블락 유효
		bIsValidBlock = UBaseFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
	
	if (bIsPlayerUnbeatable)
	{
		return;
	}
	if (bIsValidBlock)
	{
		//블락 성공 이벤트(HP대신 Stamina감소)
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, BaseGamePlayTags::Player_Event_SuccessBlock, EventData);
	}
	//플레이어가 패링중이고, 공격이 패링 가능한 공격일경우
	else if (bIsPlayerParring && bIsParryable)
	{
		//플레이어에게 패링 성공 이벤트 전송
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, BaseGamePlayTags::Shared_Event_SuccessParry, EventData );
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_MeleeHit, EventData);
	}
}

void UEnemyCombatComponent::ToggleBodyCollision(bool bUse, EToggleDamageType ToggleDamageType)
{
	Super::ToggleBodyCollision(bUse, ToggleDamageType);

	AEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AEnemyCharacter>();

	check(OwningEnemyCharacter);

	UBoxComponent* BossWeaponCollision = OwningEnemyCharacter->GetBossWeaponCollisionBox();

	check(BossWeaponCollision);

	//BossWeaponCollision->SetCollisionEnabled(bUse ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	if (bUse)
	{
		BossWeaponCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	}
	else
	{
		BossWeaponCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
		OverlappedActors.Empty();
	}
}
