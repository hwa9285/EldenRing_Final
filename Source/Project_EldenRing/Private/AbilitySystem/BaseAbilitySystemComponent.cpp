// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include "BaseGamePlayTags.h"

void UBaseAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			continue;	
		} 
		if (InputTag.MatchesTag(BaseGamePlayTags::InputTag_Toggle))
		{
			//InputTag_Toggle 태그가 있으면 Toggle On/Off 처리 
			if (Spec.IsActive())
			{
				CancelAbilityHandle(Spec.Handle);
			}
			else
			{
				TryActivateAbility(Spec.Handle);
			}
		}
		else
		{
			TryActivateAbility(Spec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	//InputTag가 Hold인것만 Released 체크
	if (!InputTag.IsValid() || !InputTag.MatchesTag(BaseGamePlayTags::InputTag_Hold))
	{
		return;
	}

	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag) && Spec.IsActive())
		{
			CancelAbilityHandle(Spec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitySet>& WeaponAbilities,
	int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (WeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FPlayerAbilitySet& WeaponAbilitySet : WeaponAbilities)
	{
		if (!WeaponAbilitySet.IsValid()) continue;

		FGameplayAbilitySpec Spec(WeaponAbilitySet.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = Level;
		Spec.DynamicAbilityTags.AddTag(WeaponAbilitySet.InputTag);
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(Spec));
        
	}
}

void UBaseAbilitySystemComponent::GrantPlayerAshOfWarAbility(const FPlayerAbilitySet& AshOfWarAbility, int32 Level,
	FGameplayAbilitySpecHandle& OutGrantedAbilitySpecHandle)
{
	if (!AshOfWarAbility.IsValid())
	{
		return;
	}
	FGameplayAbilitySpec Spec(AshOfWarAbility.AbilityToGrant);
	Spec.SourceObject = GetAvatarActor();
	Spec.Level = Level;
	Spec.DynamicAbilityTags.AddTag(AshOfWarAbility.InputTag);
	OutGrantedAbilitySpecHandle = GiveAbility(Spec);
	
}

void UBaseAbilitySystemComponent::GrantHorseAbility(const TArray<FPlayerAbilitySet>& HorseAbilitieses, int32 Level,
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandle)
{
	if (HorseAbilitieses.IsEmpty())
	{
		return;
	}

	for (const FPlayerAbilitySet& HorseAbilitySet : HorseAbilitieses)
	{
		if (!HorseAbilitySet.IsValid()) continue;

		FGameplayAbilitySpec Spec(HorseAbilitySet.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = Level;
		Spec.DynamicAbilityTags.AddTag(HorseAbilitySet.InputTag);
		OutGrantedAbilitySpecHandle.AddUnique(GiveAbility(Spec));
        
	}
}

void UBaseAbilitySystemComponent::RemoveGrantedAshOfWatAbility(FGameplayAbilitySpecHandle& SpecHandleToRemove)
{
	if (!SpecHandleToRemove.IsValid())
	{
		return;
	}
	ClearAbility(SpecHandleToRemove);
}

void UBaseAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove)
{
	if (SpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (FGameplayAbilitySpecHandle& SpecHandle : SpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	SpecHandlesToRemove.Empty();
}

void UBaseAbilitySystemComponent::RemoveGrantedHorseAbilities(TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove)
{
	if (SpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (FGameplayAbilitySpecHandle& SpecHandle : SpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	SpecHandlesToRemove.Empty();
}

bool UBaseAbilitySystemComponent::HasAbilityOfClass(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if (!AbilityClass)
	{
		return false;
	}

	// 현재 부여된 어빌리티 목록을 순회하여 같은 클래스가 있는지 확인
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass)
		{
			return true; // 이미 존재하는 경우
		}
	}

	return false; // 존재하지 않는 경우
}
bool UBaseAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag Tag)
{
	check(Tag.IsValid());

	TArray<FGameplayAbilitySpec*> AbilitySpecs;

	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tag.GetSingleTagContainer(), AbilitySpecs);

	if (!AbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, AbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* AbilitySpec = AbilitySpecs[RandomAbilityIndex];

		check(AbilitySpec);

		if (!AbilitySpec->IsActive())
		{
			return TryActivateAbility(AbilitySpec->Handle);
		}
	}
	return false;
}

void UBaseAbilitySystemComponent::CancelAbilityByTag(FGameplayTag Tag)
{
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(Tag) && Spec.IsActive())
		{
			CancelAbilityHandle(Spec.Handle);
			UE_LOG(LogTemp, Warning,TEXT("CancelAbilityByTag"));
		}
		else
		{
			UE_LOG(LogTemp, Warning,TEXT("nothing"));
		}
	}
}
