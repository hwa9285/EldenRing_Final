// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell/SpellEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGamePlayTags.h"

// Sets default values
ASpellEffect::ASpellEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASpellEffect::HandleApplyProjectile(APawn* HitPawn, FGameplayEventData& Payload)
{
	checkf(ProjectileDamageSpecHandle.IsValid(), TEXT("ProjectileDamageSpecHandle must be valid"));
	const bool bWasApplied = UBaseFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), HitPawn, ProjectileDamageSpecHandle);
	UBaseFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), HitPawn, AttributeSpecHandle);
	
	if (bWasApplied)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			BaseGamePlayTags::Shared_Event_HitReact,
			Payload
		);
	}
}


