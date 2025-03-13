// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBaseInventoryType : uint8
{
	Item UMETA(DisplayName = "Item"),
	PrimaryWeapon UMETA(DisplayName = "PrimaryWeapon"),
	SecondaryWeapon UMETA(DisplayName = "SecondaryWeapon"),
	StandbyEquipWeapon UMETA(DisplayName = "StandbyEquipWeapon"),
	Armor UMETA(DisplayName = "Armor"),
	Shield UMETA(DisplayName = "Shield"),
	Consume UMETA(DisplayName = "Consume"),
	Material UMETA(DisplayName = "Material"),
	Memory UMETA(DisplayName = "Memory"),
	EquipedMemory UMETA(DisplayName = "EquipedMemory"),
	Sword UMETA(DisplayName = "Sword"),
	Staff UMETA(DisplayName = "Staff"),
	Potion UMETA(DisplayName = "Potion"),
	SummonHorse UMETA(DisplayName = "SummonHorse"),
	AllequipedWeapon UMETA(DisplayName = "AllequipedWeapon"),
	MakeItem UMETA(DisplayName = "MakeItem"),
};

UENUM()
enum class EBaseConfirmType : uint8
{
	Yes,
	No,
};

UENUM()
enum class EBaseValidType : uint8
{
	Valid,
	Invalid,
};

UENUM()
enum class EBaseSuccessType : uint8
{
	Success,
	Failed,
};

UENUM(BlueprintType)
enum class EStatsType : uint8
{
	Vitality,
	Mentality,
	Endurance,
	Strength,
	Agility,
	Intellect,
	Faith,
	Mystery,
	Level
};

UENUM(BlueprintType)
enum class EAttributeName : uint8
{
	MaxHp,
	MaxMp,
	Attack,
	LeftAttack,
	BaseAttack,
	Defence,
	MaxStamina,
	Rune
};

UENUM(BlueprintType)
enum class ESpawnWeaponType : uint8
{
	Left,
	Right,
};

UENUM(BlueprintType)
enum class EAttackWidgetType : uint8
{
	None,
	Left,
	Right,
};

UENUM(BlueprintType)
enum class EDirectionType : uint8
{
	Front,
	Back,
	Left,
	Right,
	None,
};

UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
	OnHit,
	OnBeginOverlap,
};


//스펠이 마술인지 기도인지
UENUM(BlueprintType)
enum class ESpellCategory : uint8
{
	Magic,
	Pray
};

//공격형인지 지원형인지
UENUM(BlueprintType)
enum class ESpellType : uint8
{
	Attack,
	Support
};

//장판형인지 투사체인지
UENUM(BlueprintType)
enum class EAttackForm : uint8
{
	Area,
	Projectile
};

//버프형인지 회복형인지
UENUM(BlueprintType)
enum class ESupportForm : uint8
{
	Recovery,
	Buff
};

//실행 타입
UENUM(BlueprintType)
enum class ESpellExecutionCategory : uint8
{
	Failed,
	AttackArea,
	AttackProjectile,
	SupportBuff,
	SupportRecovery
};


UENUM(BlueprintType)
enum class EEnemyStateType : uint8
{
	Idle,
	Battle,
	Groggy,
};



//임시로 추가
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon,
	Armor,
	Ingredient,
	Consumption,
	Memory,
};

UENUM(BlueprintType)
enum class EWeaponStatType : uint8
{
	None,
	E,
	D,
	C,
	B,
	A
};

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	PickUpItem,
	SavePoint,
	Talk,
	None,
};

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Bleed,
	Burn,
	DeBuffAtk,
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Attack,
	Parry,
	None
};
UENUM(BlueprintType)
enum class ESkillAdditionalType:uint8
{
	Blow,
	Projectile
};

UENUM(BlueprintType)
enum class EConsumeType:uint8
{
	Horse,
	Potion
};