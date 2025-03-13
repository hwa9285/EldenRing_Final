// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HumanCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseDebugHelper.h"
#include "BaseFunctionLibrary.h"
#include "AbilitySystem/PlayerAttributeSet.h"
#include "Character/PlayerCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Interfaces/PawnUIInterface.h"
#include "Item/Equipment/Weapon/PlayerWeaponBase.h"
#include "Item/Subsystem/InventoryGameInstanceSubsystem.h"
#include "SubSystem/SaveSubsystem.h"

#pragma region Weapon Functions

UHumanCombatComponent::UHumanCombatComponent()
{
	CurrentEquipSpellIndex = 0;
	PlayerCarriedSpell.Init(NAME_None,6);

	//임시로 초기화
	CurrentEquipItemIndex = 0;
	FConsumeInfo InitItem;
	InitItem.ConsumeID = NAME_None;
	InitItem.Quantity = 0;
	InitItem.Icon = nullptr;
	InitItem.ConsumeType = EConsumeType::Horse;
	CarriedItem.Init(InitItem,3);
}

void UHumanCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);
    
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UE_LOG(LogTemp, Warning, TEXT("OnHitActor"));
	
	//패링 가능한 공격이면 다른 태그 보내기
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), bIsParryable? BaseGamePlayTags::Shared_Event_MeleeHit_Parryable: BaseGamePlayTags::Shared_Event_MeleeHit, Data);
}


float UHumanCombatComponent::GetHeroCurrentEquippedWeaponDamageAtLevel(float Level) const
{
	//return GetPlayerCurrentEquippedWeapon()->WeaponData.TotalDamage;
	return 0.f;
}


//오른손 함수
void UHumanCombatComponent::TogglePlayerWeaponCollision(bool bUse, EToggleDamageType ToggleDamagetype)
{
	if (ToggleDamagetype == EToggleDamageType::CurrentEquippedWeapon)
	{
		APlayerWeaponBase* Weapon = GetCurrentEquipRightWeaponByIndex(CurrentEquipRightWeaponIndex);
	
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
}

//왼손함수
void UHumanCombatComponent::TogglePlayerLeftWeaponCollision(bool bUse, EToggleDamageType ToggleDamagetype)
{
	if (ToggleDamagetype == EToggleDamageType::CurrentEquippedWeapon)
	{
		APlayerWeaponBase* Weapon = GetCurrentEquipLeftWeaponByIndex(CurrentEquipLeftWeaponIndex);
	
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
}

TMap<int32, APlayerWeaponBase*> UHumanCombatComponent::GetCurrentCarriedMapByType(ESpawnWeaponType Type)
{
	switch (Type)
	{
	case ESpawnWeaponType::Left:
		return PlayerCarriedSecondaryWeaponMap;
	case ESpawnWeaponType::Right:
		return PlayerCarriedPrimaryWeaponMap;
	default:
		return PlayerCarriedPrimaryWeaponMap;
	}
}


void UHumanCombatComponent::RegisterPlayerWeaponSpawn(int32 SlotIndex, ESpawnWeaponType Type, APlayerWeaponBase* Weapon,
                                                      bool bRegisterAsPlayerEquippedWeapon)
{
	TMap<int32, APlayerWeaponBase*> &CarriedWeaponMap = (Type == ESpawnWeaponType::Left) 
		? PlayerCarriedSecondaryWeaponMap 
		: PlayerCarriedPrimaryWeaponMap;

	//이미 값이 존재하면, 그냥 덮어씌우면 됨. 값처리는 인벤이 할거임.
	
	//checkf(!CarriedWeaponMap.Contains(SlotIndex),TEXT("already has index weapon"));
	check(Weapon)
	
	CarriedWeaponMap.Add(SlotIndex,Weapon);
	
	Weapon->OnWeaponHitTarget.BindUObject(this,&UPawnCombatComponent::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this,&UPawnCombatComponent::OnWeaponPulledFromTargetActor);
	
}

void UHumanCombatComponent::RemovePlayerWeapon(int32 SlotIndex, ESpawnWeaponType _Type)
{
	TMap<int32, APlayerWeaponBase*> &CarriedWeaponMap = (_Type == ESpawnWeaponType::Left) 
		? PlayerCarriedSecondaryWeaponMap 
		: PlayerCarriedPrimaryWeaponMap;
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPawn());

	switch (_Type)
	{
	case ESpawnWeaponType::Left:
		if (CurrentEquipLeftWeaponTag != FGameplayTag::EmptyTag)
		{
			CurrentEquipLeftWeaponTag = FGameplayTag::EmptyTag;
			UE_LOG(LogTemp,Warning,TEXT("Left, NotEmpty"))
		}
		PlayerCharacter->UnequipHandle(_Type,CarriedWeaponMap[SlotIndex]);
		CarriedWeaponMap[SlotIndex]->Destroy();
		CarriedWeaponMap.Remove(SlotIndex);
		break;
	case ESpawnWeaponType::Right:
		if (CurrentEquipRightWeaponTag != FGameplayTag::EmptyTag)
		{
			CurrentEquipRightWeaponTag = FGameplayTag::EmptyTag;
			UE_LOG(LogTemp,Warning,TEXT("Right, NotEmpty"))
		}
		PlayerCharacter->UnequipHandle(_Type,CarriedWeaponMap[SlotIndex]);
		CarriedWeaponMap[SlotIndex]->Destroy();
		CarriedWeaponMap.Remove(SlotIndex);
		break;
	}
}

APlayerWeaponBase* UHumanCombatComponent::GetCurrentEquipRightWeaponByIndex(int32 SlotIndex)
{
	if (*PlayerCarriedPrimaryWeaponMap.Find(SlotIndex))
	{
		return *PlayerCarriedPrimaryWeaponMap.Find(SlotIndex);	
	}
	return nullptr;
}

APlayerWeaponBase* UHumanCombatComponent::GetCurrentEquipLeftWeaponByIndex(int32 SlotIndex)
{
	if (*PlayerCarriedSecondaryWeaponMap.Find(SlotIndex))
	{
		return *PlayerCarriedSecondaryWeaponMap.Find(SlotIndex);	
	}
	return nullptr;
}
#pragma endregion


#pragma region Spell Functions

//인벤토리에서 자동 실행, 마법 등록
void UHumanCombatComponent::RegisterCarriedSpell(uint8 Slot, FName SpellID)
{
	PlayerCarriedSpell[Slot] = SpellID;
	UpdateSpellExecutionData();
}

FName UHumanCombatComponent::GetCurrentEquipSpellID()
{
	return PlayerCarriedSpell[CurrentEquipSpellIndex];
}

//무기변경이나 마법 변경이 일어날 때 미리 계산
void UHumanCombatComponent::UpdateSpellExecutionData()
{

	if (!CachedUIInterface.IsValid())
	{
	 	CachedUIInterface = TWeakInterfacePtr<IPawnUIInterface>(GetOwner());
	}
	//
	check(CachedUIInterface.IsValid());
	
	
	if (GetCurrentEquipSpellID() == NAME_None)
	{
		CurrentSpellExecutionData.bIsEnoughFP = false;
		CurrentSpellExecutionData.bLeftHandCanExecute = false;
		CurrentSpellExecutionData.bRightHandCanExecute = false;
		CurrentSpellExecutionData.SpellExecutionCategory = ESpellExecutionCategory::Failed;

		if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
		{
			PlayerUIComponent->	OnCheckedSpell.Broadcast(CurrentSpellExecutionData.bLeftHandCanExecute,CurrentSpellExecutionData.bRightHandCanExecute,nullptr);
		}
		
		return;
	}
	UInventoryGameInstanceSubsystem* Subsystem= GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	FPlayerSpellInfo CurrentSpellInfo = Subsystem->GetPlayerSpellData(GetCurrentEquipSpellID());

	//마나가 충분한지 확인
	IsEnoughFP();
	
	
	switch (CurrentSpellInfo.SpellCategory)
	{
	case ESpellCategory::Magic:
		if (CurrentEquipLeftWeaponTag == BaseGamePlayTags::Equipment_Staff)
		{
			CurrentSpellExecutionData.bLeftHandCanExecute = true;
		}
		else
		{
			CurrentSpellExecutionData.bLeftHandCanExecute = false;
		}
		
		if (CurrentEquipRightWeaponTag == BaseGamePlayTags::Equipment_Staff)
		{
			CurrentSpellExecutionData.bRightHandCanExecute = true;
		}
		else
		{
			CurrentSpellExecutionData.bRightHandCanExecute = false;
		}
		break;
	case ESpellCategory::Pray:
		if (CurrentEquipLeftWeaponTag == BaseGamePlayTags::Equipment_SacredSeal)
		{
			CurrentSpellExecutionData.bLeftHandCanExecute = true;
		}
		else
		{
			CurrentSpellExecutionData.bLeftHandCanExecute = false;
		}
		
		if (CurrentEquipRightWeaponTag == BaseGamePlayTags::Equipment_SacredSeal)
		{
			CurrentSpellExecutionData.bRightHandCanExecute = true;
		}
		else
		{
			CurrentSpellExecutionData.bRightHandCanExecute = false;
		}
		break;
	}

	switch (CurrentSpellInfo.SpellType)
	{
	case ESpellType::Attack:
		switch (CurrentSpellInfo.AttackForm)
		{
		case EAttackForm::Area:
			CurrentSpellExecutionData.SpellExecutionCategory = ESpellExecutionCategory::AttackArea;
			break;

		case EAttackForm::Projectile:
			CurrentSpellExecutionData.SpellExecutionCategory = ESpellExecutionCategory::AttackProjectile;
			break;
		}
		break;
	case ESpellType::Support:
		switch (CurrentSpellInfo.SupportForm)
		{
		case ESupportForm::Buff:
			CurrentSpellExecutionData.SpellExecutionCategory = ESpellExecutionCategory::SupportBuff;
			break;

		case ESupportForm::Recovery:
			CurrentSpellExecutionData.SpellExecutionCategory = ESpellExecutionCategory::SupportRecovery;
			break;
		}
		break;
	}
	
	if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
	{
		PlayerUIComponent->	OnCheckedSpell.Broadcast(CurrentSpellExecutionData.bLeftHandCanExecute,CurrentSpellExecutionData.bRightHandCanExecute,CurrentSpellInfo.Icon);
	}
}

void UHumanCombatComponent::IsEnoughFP()
{
	if (GetCurrentEquipSpellID() == NAME_None)
	{
		CurrentSpellExecutionData.bIsEnoughFP = false;
		CurrentSpellExecutionData.bLeftHandCanExecute = false;
		CurrentSpellExecutionData.bRightHandCanExecute = false;
		CurrentSpellExecutionData.SpellExecutionCategory = ESpellExecutionCategory::Failed;
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPawn());
	UInventoryGameInstanceSubsystem* Subsystem= GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
	
	if (Subsystem->GetPlayerSpellData(GetCurrentEquipSpellID()).CostFP <= PlayerCharacter->GetBaseAttributeSet()->GetCurrentFP())
	{
		CurrentSpellExecutionData.bIsEnoughFP = true;
	}
	else
	{
		CurrentSpellExecutionData.bIsEnoughFP = false;
	}
	
	if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
	{
		PlayerUIComponent->OnCheckCanUseSkill.Broadcast(CurrentSpellExecutionData.bIsEnoughFP);
	}
}

#pragma endregion


#pragma region ItemFunctions

void UHumanCombatComponent::RegisterItem(uint8 SlotIndex, FConsumeInfo ItemInfo)
{
	CarriedItem[SlotIndex] = ItemInfo;
	//착용한 아이템 위젯 업데이트(아이콘, 개수)

	UpdateEquipItemWidget();
}

bool UHumanCombatComponent::CheckAvailableItem()
{
	if (CarriedItem[CurrentEquipItemIndex].Quantity > 0)
	{
		return true;
	}
	return false;
}

void UHumanCombatComponent::UpdateEquipItemWidget()
{
	if (!CachedUIInterface.IsValid())
	{
		CachedUIInterface = TWeakInterfacePtr<IPawnUIInterface>(GetOwner());
	}
	check(CachedUIInterface.IsValid());
	
	if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
	{
		PlayerUIComponent->OnUpdateItem.Broadcast(CarriedItem[CurrentEquipItemIndex].ConsumeType,CarriedItem[CurrentEquipItemIndex].Icon,CarriedItem[CurrentEquipItemIndex].Quantity);
	}
}

void UHumanCombatComponent::UpdateCurrentQuantity()
{
	CarriedItem[CurrentEquipItemIndex].Quantity -= 1;
	//인벤토리에도 반영 해야하면 같은 델리게이트로 인벤토리 위젯에서도 바인드 걸어두기
	if (UPlayerUIComponent* PlayerUIComponent = GetPawnUIInterface()->GetPlayerUIComponent())
	{
		//위젯 내에서 0이면 투명도 낮추는 작업 ㄱㄱ
		PlayerUIComponent->OnUpdateItemQuantity.Broadcast(CarriedItem[CurrentEquipItemIndex].ConsumeID,CarriedItem[CurrentEquipItemIndex].Quantity);
	}
}

//아이템을 사용하기위해선 사용가능한지 판단하고 -> CheckAvailableItem이용
//CheckAvailableItem의 값에따라 위젯도 업데이트 해야함.


#pragma endregion

void UHumanCombatComponent::SaveData()
{
	USaveSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveSubsystem>();
	Subsystem->SaveCombatComponent(this);
}

void UHumanCombatComponent::RegisterPlayerWeaponByNewPawn()
{
	FAttachmentTransformRules AttachmentRules = {EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,true};
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	for (auto& RightWeapon : PlayerCarriedPrimaryWeaponMap)
	{
		if (RightWeapon.Value)
		{
			RightWeapon.Value->SetOwner(GetOwner());
			RightWeapon.Value->SetInstigator(Cast<APawn>(GetOwner()));
			RightWeapon.Value->OnWeaponHitTarget.BindUObject(this,&UPawnCombatComponent::OnHitTargetActor);
			RightWeapon.Value->OnWeaponPulledFromTarget.BindUObject(this,&UPawnCombatComponent::OnWeaponPulledFromTargetActor);
			
			//Hidden상태 일 경우, 장착중이던 무기가 아니므로 플레이어 소켓에 부착만 하기
			if (RightWeapon.Value->IsHidden())
			{
				RightWeapon.Value->AttachToComponent(PlayerCharacter->GetMesh(),AttachmentRules,"WeaponSocket");
			}
			else
			{
				PlayerCharacter->EquipHandle(ESpawnWeaponType::Right,RightWeapon.Value);
				RightWeapon.Value->AttachToComponent(PlayerCharacter->GetMesh(),AttachmentRules,"hand_rSocket");
				PlayerCharacter->SetEquipIndexFromType(ESpawnWeaponType::Right,CurrentEquipRightWeaponIndex);
			}
		}
	}
	for (auto& LeftWeapon : PlayerCarriedSecondaryWeaponMap)
	{
		if (LeftWeapon.Value)
		{
			LeftWeapon.Value->SetOwner(GetOwner());
			LeftWeapon.Value->SetInstigator(Cast<APawn>(GetOwner()));
			LeftWeapon.Value->OnWeaponHitTarget.BindUObject(this,&UPawnCombatComponent::OnHitTargetActor);
			LeftWeapon.Value->OnWeaponPulledFromTarget.BindUObject(this,&UPawnCombatComponent::OnWeaponPulledFromTargetActor);
			
			//Hidden상태 일 경우, 장착중이던 무기가 아니므로 플레이어 소켓에 부착만 하기
			if (LeftWeapon.Value->IsHidden())
			{
				LeftWeapon.Value->AttachToComponent(PlayerCharacter->GetMesh(),AttachmentRules,"WeaponSocket");
			}
			else
			{
				PlayerCharacter->EquipHandle(ESpawnWeaponType::Left,LeftWeapon.Value);
				LeftWeapon.Value->AttachToComponent(PlayerCharacter->GetMesh(),AttachmentRules,"hand_lSocket");
				PlayerCharacter->SetEquipIndexFromType(ESpawnWeaponType::Left,CurrentEquipLeftWeaponIndex);
				
			}
		}
	}
}

void UHumanCombatComponent::InitializeEquipItem()
{
	UpdateEquipItemWidget();
}
