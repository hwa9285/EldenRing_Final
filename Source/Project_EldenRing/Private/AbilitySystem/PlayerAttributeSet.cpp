// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PlayerAttributeSet.h"

#include "BaseFunctionLibrary.h"
#include "GameplayEffectExtension.h"
#include "Components/Stat/StatComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	InitMaxStamina(1.0f);
	InitCurrentStamina(1.0f);
	InitMaxFP(1.0f);
	InitCurrentFP(1.0f);
	InitLeftHandAttack(1.f);
	InitLeftHandCostStamina(0.f);
	InitRightHandCostStamina(0.f);
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetMaxHpAttribute())
	{
		const float NewMaxHp = GetMaxHp();
		SetCurrentHp(NewMaxHp);
		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnMaxHpChanged.Broadcast(NewMaxHp);	
		}
	}
	if (Data.EvaluatedData.Attribute == GetMaxFPAttribute())
	{
		const float NewMaxFP = GetMaxFP();
		SetCurrentFP(NewMaxFP);
		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnMaxFPChanged.Broadcast(NewMaxFP);	
		}
	}
	if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		const float NewMaxStamina = GetMaxStamina();
		SetCurrentStamina(NewMaxStamina);
		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnMaxStaminaChanged.Broadcast(NewMaxStamina);	
		}
	}
	

	if (Data.EvaluatedData.Attribute == GetCurrentHpAttribute())
	{
		const float NewCurrentHp = FMath::Clamp(GetCurrentHp(),0.0f,GetMaxHp());
		SetCurrentHp(NewCurrentHp);
		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentHPChanged.Broadcast(GetCurrentHp()/GetMaxHp());	
		}
	}
	
	//FP
	if (Data.EvaluatedData.Attribute == GetCurrentFPAttribute())
	{
		const float NewCurrentFP = FMath::Clamp(GetCurrentFP(),0.0f,GetMaxFP());
		SetCurrentFP(NewCurrentFP);
		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentFPChanged.Broadcast(GetCurrentFP()/GetMaxFP());	
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		float NewStamina = FMath::Clamp(GetCurrentStamina(), 0.0f, GetMaxStamina());
		SetCurrentStamina(NewStamina);

		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());	
		} 
	}

	//플레이어가 룬을 얻었을 때, 
	if (Data.EvaluatedData.Attribute == GetRuneAttribute())
	{
		//float OldRune = GetRune();
		float NewRune = GetRune();
		SetRune(NewRune);

		if (UStatComponent* PlayerStatComponent = GetPawnUIInterface()->GetStatComponent())
		{
			PlayerStatComponent->OnPlayerRuneChanged.Broadcast(GetRune());
		}
	}

	if (Data.EvaluatedData.Attribute == GetBaseAttackAttribute())
	{
		SetAttack(GetBaseAttack() + GetRightWeaponAttack());
		SetLeftHandAttack(GetBaseAttack() + GetLeftWeaponAttack());
		if (UStatComponent* PlayerStatComponent = GetPawnUIInterface()->GetStatComponent())
		{
			PlayerStatComponent->ResetPendingStats();
			UE_LOG(LogTemp, Warning, TEXT("BaseAttack Changed"));
		}
	}
	if (Data.EvaluatedData.Attribute == GetLeftWeaponAttackAttribute())
	{
		SetLeftHandAttack(GetBaseAttack() + GetLeftWeaponAttack());
		if (UStatComponent* PlayerStatComponent = GetPawnUIInterface()->GetStatComponent())
		{
			PlayerStatComponent->ResetPendingStats();
			UE_LOG(LogTemp, Warning, TEXT("LeftWeaponAttack Changed"));
		}
	}
	if (Data.EvaluatedData.Attribute == GetRightWeaponAttackAttribute())
	{
		SetAttack(GetBaseAttack() + GetRightWeaponAttack());
		if (UStatComponent* PlayerStatComponent = GetPawnUIInterface()->GetStatComponent())
		{
			PlayerStatComponent->ResetPendingStats();
			UE_LOG(LogTemp, Warning, TEXT("RightWeaponAttack Updated"));
		}
	}
	if (Data.EvaluatedData.Attribute == GetAttackAttribute())
	{
		UE_LOG(LogTemp,Warning,TEXT("TEXT"));
	}
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute.GetName() == TEXT("Attack"))
	{
		float CurrentBaseAttack = GetBaseAttack() + GetRightWeaponAttack();
		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnUpdateAttackAdditional.Broadcast(Attribute.GetNumericValue(this) - CurrentBaseAttack ,ESpawnWeaponType::Right);	
		}
	}
	if (Attribute.GetName() == TEXT("LeftHandAttack"))
	{
		float CurrentLeftBaseAttack = GetBaseAttack() + GetLeftWeaponAttack();
		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnUpdateAttackAdditional.Broadcast(Attribute.GetNumericValue(this) - CurrentLeftBaseAttack,ESpawnWeaponType::Left);	
		}
	}
}
