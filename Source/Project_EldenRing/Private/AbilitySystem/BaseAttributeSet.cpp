// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/BaseAttributeSet.h"

#include"GameplayEffectExtension.h"
#include "BaseGamePlayTags.h"
#include "BaseFunctionLibrary.h"

#include "Components/UI/PawnUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

#include "BaseDebugHelper.h"

UBaseAttributeSet::UBaseAttributeSet()
{
	InitMaxHp(1.0f);
	InitCurrentHp(1.0f);
	InitAttack(1.0f);
	InitMaxGroggy(1.0f);
	InitCurrentGroggy(1.0f);
	InitDamageTaken(1.f);
	InitDefence(10.f);
	InitImpulse(1.f);
	InitCurrentBurn(1.f);
	InitMaxBurn(1.f);
	InitCurrentBleed(1.f);
	InitMaxBleed(1.f);
	InitCurrentAtkDeBuff(1.f);
	InitMaxAtkDeBuff(1.f);
	InitGroggyTaken(1.f);
	InitBleedTaken(1.f);
	InitBurnTaken(1.f);
	InitDebuffAtkTaken(1.f);
	InitRune(1.f);
	InitTakenBleedLev(1.f);
	InitTakenBurnLev(1.f);
	InitTakenDebuffAtkLev(1.f);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (!CachedUIInterface.IsValid())
	{
		CachedUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	
	checkf(CachedUIInterface.IsValid(),TEXT("%s does not Implemetation IPawnUIComponent"),*Data.Target.GetAvatarActor()->GetActorLabel());
	UPawnUIComponent* PawnUIComponent = CachedUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent,TEXT("Can not load PawnUIComponent from %s"),*Data.Target.GetAvatarActor()->GetActorLabel());
	
	if (Data.EvaluatedData.Attribute == GetCurrentHpAttribute())
	{
		const float NewCurrentHp = FMath::Clamp(GetCurrentHp(),0.0f,GetMaxHp());
		SetCurrentHp(NewCurrentHp);
		if (PawnUIComponent)
		{
			UE_LOG(LogTemp,Warning,TEXT("HP:%f | Percent: %f"),GetCurrentHp(),GetCurrentHp()/GetMaxHp());
			PawnUIComponent->OnCurrentHPChanged.Broadcast(GetCurrentHp()/GetMaxHp());	
		}
	}
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float BeforeHp = GetCurrentHp();
		const float Damage = GetDamageTaken();

		const float NewCurrentHp = FMath::Clamp(BeforeHp - Damage, 0.0f, GetMaxHp());
		SetCurrentHp(NewCurrentHp);
		
		PawnUIComponent->OnCurrentHPChanged.Broadcast(GetCurrentHp() / GetMaxHp());
		PawnUIComponent->OnDamageTakenChanged.Broadcast(Damage);
		
		//Character Death Process
		if (NewCurrentHp == 0.f)    
		{
			UBaseFunctionLibrary::AddGameplayTagToActor(Data.Target.GetAvatarActor(), BaseGamePlayTags::Shared_Status_Dead);
			//죽었을 때, 룬이 플레이어에게 들어오게 해야함.
		}
	}
	if (Data.EvaluatedData.Attribute == GetGroggyTakenAttribute())
	{
		const float BeforeGroggy = GetCurrentGroggy();
		const float Groggy = GetGroggyTaken();

		const float NewCurrentGroggy = FMath::Clamp(BeforeGroggy - Groggy, 0.0f, GetMaxGroggy());
		SetCurrentGroggy(NewCurrentGroggy);

		//const FString DebugString = FString::Printf(TEXT("BeforeGroggy: %f, Groggy: %f, NewCurrentGroggy : %f"), BeforeGroggy, Groggy, NewCurrentGroggy);
		//Debug::Print(DebugString, FColor::Green);
		
		if (NewCurrentGroggy == 0.f)    
		{
			UBaseFunctionLibrary::AddGameplayTagToActor(Data.Target.GetAvatarActor(), BaseGamePlayTags::Shared_Status_Groggy);
		}
	}
	if (Data.EvaluatedData.Attribute == GetCurrentGroggyAttribute())
	{
		const float NewCurrentGroggy = FMath::Clamp(GetCurrentGroggy(),0.0f,GetMaxGroggy());
		SetCurrentGroggy(NewCurrentGroggy);
	}

	
	//속성공격
	if (Data.EvaluatedData.Attribute == GetBurnTakenAttribute())
	{
		const float BeforeBurn = GetCurrentBurn();
		const float NewBurnTaken = GetBurnTaken();
		const float NewBurn = FMath::Clamp(BeforeBurn - NewBurnTaken, 0.0f, GetMaxBurn());
		SetCurrentBurn(NewBurn);

		//const FString DebugString = FString::Printf(TEXT("BeforeBurn: %f, BurnTaken: %f, NewBurn : %f"), BeforeBurn, NewBurnTaken, NewBurn);
		//Debug::Print(DebugString, FColor::Green);

		UBaseFunctionLibrary::NativeGetAbilitySystemComponentFromActor(Data.Target.GetAvatarActor())->TryActivateAbilityByTag(BaseGamePlayTags::Shared_Ability_Block_Recovery_Burn);
		
		if (NewBurn == 0.f)    
		{
			UBaseFunctionLibrary::NativeGetAbilitySystemComponentFromActor(Data.Target.GetAvatarActor())->TryActivateAbilityByTag(BaseGamePlayTags::Shared_Ability_Burn);
		}
	}
	if (Data.EvaluatedData.Attribute == GetCurrentBurnAttribute())
	{
		const float NewCurrentBurn = FMath::Clamp(GetCurrentBurn(),0.0f,GetMaxBurn());
		SetCurrentBurn(NewCurrentBurn);
	}
	
	
	if (Data.EvaluatedData.Attribute == GetDebuffAtkTakenAttribute())
	{
		const float BeforeDebuff = GetCurrentAtkDeBuff();
		const float NewDebuffTaken = GetDebuffAtkTaken();
		const float NewDefuff = FMath::Clamp(BeforeDebuff - NewDebuffTaken, 0.0f, GetMaxAtkDeBuff());
		SetCurrentAtkDeBuff(NewDefuff);

		UBaseFunctionLibrary::NativeGetAbilitySystemComponentFromActor(Data.Target.GetAvatarActor())->TryActivateAbilityByTag(BaseGamePlayTags::Shared_Ability_Block_Recovery_DeBuffAtk);
		
		if (NewDefuff == 0.f)    
		{
			UBaseFunctionLibrary::NativeGetAbilitySystemComponentFromActor(Data.Target.GetAvatarActor())->TryActivateAbilityByTag(BaseGamePlayTags::Shared_Ability_Debuff_Atk);
		}
	}
	if (Data.EvaluatedData.Attribute == GetCurrentAtkDeBuffAttribute())
	{
		const float NewCurrentAtkDeBuff = FMath::Clamp(GetCurrentAtkDeBuff(),0.0f,GetMaxAtkDeBuff());
		SetCurrentAtkDeBuff(NewCurrentAtkDeBuff);
	}
	
	if (Data.EvaluatedData.Attribute == GetBleedTakenAttribute())
	{
		const float BeforeBleed = GetCurrentBleed();
		const float NewBleedTaken = GetBleedTaken();
		const float NewBleed = FMath::Clamp(BeforeBleed - NewBleedTaken, 0.0f, GetMaxBleed());
		SetCurrentBleed(NewBleed);

		UBaseFunctionLibrary::NativeGetAbilitySystemComponentFromActor(Data.Target.GetAvatarActor())->TryActivateAbilityByTag(BaseGamePlayTags::Shared_Ability_Block_Recovery_Bleed);
		
		if (NewBleed == 0.f)    
		{
			UBaseFunctionLibrary::NativeGetAbilitySystemComponentFromActor(Data.Target.GetAvatarActor())->TryActivateAbilityByTag(BaseGamePlayTags::Shared_Ability_Bleed);
		}
	}
	if (Data.EvaluatedData.Attribute == GetCurrentBleedAttribute())
	{
		const float NewCurrentBleed = FMath::Clamp(GetCurrentBleed(),0.0f,GetMaxBleed());
		SetCurrentBleed(NewCurrentBleed);
	}
	
	//PawnUIComponent->OnCurrentHPChanged.Broadcast(CurrentStamina.GetCurrentValue() / MaxStamina.GetCurrentValue());
}






