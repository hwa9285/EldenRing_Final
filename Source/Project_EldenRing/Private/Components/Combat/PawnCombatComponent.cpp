// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"

#include "BaseDebugHelper.h"
#include "Components/BoxComponent.h"
#include "Item/Equipment/WeaponBase.h"
#include "Item/Equipment/Weapon/PlayerWeaponBase.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWeaponBase* Weapon, bool bRegisterAsEquippedWeapon)
{
	check(Weapon);

	CharacterCarriedWeaponMap.Emplace(WeaponTag,Weapon);
	
	Weapon->OnWeaponHitTarget.BindUObject(this,&UPawnCombatComponent::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this,&UPawnCombatComponent::OnWeaponPulledFromTargetActor);
	
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = WeaponTag;
	}
}

AWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(WeaponTag))
	{
		if (AWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(WeaponTag))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

FGameplayTag UPawnCombatComponent::GetCurrentEquipTag()
{
	UE_LOG(LogTemp,Warning,TEXT("Current Tag: %s"),*CurrentEquippedWeaponTag.ToString());
	return CurrentEquippedWeaponTag;
}

AWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (CurrentEquippedWeaponTag.IsValid() == false)
	{
		return nullptr;
	}
	
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bUse, EToggleDamageType ToggleDamagetype)
{
	if (ToggleDamagetype == EToggleDamageType::CurrentEquippedWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bUse);
	}
	else
	{
		ToggleBodyCollision(bUse,ToggleDamagetype);
	}
}




void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	//자식에서 구현
}

void UPawnCombatComponent::SetParryable(bool bNewParryable)
{
	bIsParryable = bNewParryable;
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}

void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bUse)
{
	AWeaponBase* Weapon = GetCharacterCurrentEquippedWeapon();

	check(Weapon);
	
	if (bUse)
	{
		Weapon->GetItemCollsionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		Weapon->GetItemCollsionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlappedActors.Empty();
	}
}

void UPawnCombatComponent::ToggleBodyCollision(bool bUse, EToggleDamageType ToggleDamageType)
{
	//자식에서 구현(EnemyCombatComponent)
}
