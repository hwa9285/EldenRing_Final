// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BaseEnum.h"
#include "Engine/DataTable.h"
#include "PaperSprite.h"
#include "CoreMinimal.h"
#include "BaseGamePlayTags.h"
#include "ScalableFloat.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"
#include "BaseStruct.generated.h"



class UGameplayEffect;
class UNiagaraSystem;
class UNiagaraComponent;
class ASpellEffect;
class AProjectileBase;
class AItemBase;
class UPlayerLinkedAnimLayer;
class UInputMappingContext;
class AWeaponBase;
class APlayerWeaponBase;


#pragma region Items

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase  // FTableRowBase를 상속
{
	GENERATED_BODY()

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Name; 

	//웨폰타입 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FGameplayTag WeaponType;
	
	// 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FText Description;  

	// 아이템 아이콘(FSlateBrush 로 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FSlateBrush Thumbnail;


	// 최대 아이템 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MaximumQuantity;

	// 인벤토리 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EBaseInventoryType BaseInventory;

	//ItemType
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag ItemType;
	
	// 아이템 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<AItemBase> ItemActorClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FGuid ItemGuid;
	
	FItemData()
		: Name((TEXT("Default Description"))),
		Description(FText::FromString(TEXT("Default Description"))),
		Thumbnail(),
		MaximumQuantity(1),
		BaseInventory(EBaseInventoryType::PrimaryWeapon),
	    ItemType(BaseGamePlayTags::Item_PrimaryWeapon),
	    ItemActorClass(nullptr),
	    ItemGuid()
	{
		Thumbnail.SetResourceObject(nullptr);  // 명시적으로 리소스 제거
		Thumbnail.ImageSize = FVector2D(0, 0); // 크기 초기화 (선택)
	}
};


USTRUCT(BlueprintType)
struct FSlotData : public FItemData
{
	GENERATED_BODY()

	//아이템 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
	FName ItemID;

	//인벤토리에 있는 현재 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
	int32 Quantity;

	//오브젝트 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
	int32 ObjectID;

};



USTRUCT(BlueprintType)
struct FAttackPower
{
	GENERATED_BODY()

	//물리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackPower")
	float Phy;

	//마법
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackPower")
	float Mag;

	//화염
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackPower")
	float Fire;

	//벼락
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackPower")
	float Light;

	//신성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackPower")
	float Holy;

	//크리티컬 피해는 무기의 일반 기본 데미지를 곱하여 크리티컬 공격이 일반 공격보다 몇 배 더 많은 피해를 입힙니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackPower")
	float CriticalDamage;
	//Highland Axe는 110% 또는 1.1x 승수를 의미하는 "110"의 크리티컬 스탯을 나열합니다.
	//치명타 공격을 수행할 수 있는 몇 가지 상황
	//1. 패링 후 비틀거릴때
	//2. 자고 있는 적을 공격 시
	//3. 말에서 내릴때
};

USTRUCT(BlueprintType)
struct FGuardReduction
{
	GENERATED_BODY()

	//물리 경감률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuardReduction")
	float PhyReduction;

	//마법 경감률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuardReduction")
	float MagReduction;

	//화염 경감률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuardReduction")
	float FireReduction;

	//벼락 경감률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuardReduction")
	float LightReduction;

	//신성 경감률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuardReduction")
	float HolyReduction;

	//가드 강도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuardReduction")
	float GuardToughness;
};


USTRUCT(BlueprintType)
struct FMakeItemInfo: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NeedItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NeedQuantity;
};


#pragma endregion

USTRUCT(BlueprintType)
struct FPlayerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase  // FTableRowBase를 상속
{
	GENERATED_BODY()
	
	//무기 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FGameplayTag AbilityTag;

	//장착 시 부여할 능력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	TArray<FPlayerAbilitySet> WeaponAbilities;
	
    //장착 몽타주
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animation")
	TObjectPtr<UAnimMontage> EquipMontage;

	//탈착 몽타주
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animation")
	TObjectPtr<UAnimMontage> UnequipMontage;

	//애님레이어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	TSubclassOf<UPlayerLinkedAnimLayer> AnimLayerToLink;

	// 입력 매핑 컨텍스트 오브젝트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	UInputMappingContext* InputMappingContext;
	
	//총합공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	float TotalDamage;
	
	//중량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	float Weight;

	//소비 FP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	float CostFP;

	//공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FAttackPower AttackPower;

	//가드 시 감소율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FGuardReduction GuardReduction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	TSubclassOf<APlayerWeaponBase> PlayerWeaponBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	UPaperSprite* Sprite;
};


USTRUCT(BlueprintType)
struct FWeaponItemInfo  //: public FTableRowBase  // FTableRowBase를 상속
{
	GENERATED_BODY()

	// 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemID;
	
	// 아이템 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<APlayerWeaponBase> ItemActorClass; 

	// 강화 시 저장할 무기 데이터 (예: 공격력, 내구도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FWeaponData WeaponRightData; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FWeaponData WeaponLeftData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bIsValid = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bIsEquip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FGuid ItemGUID;
	
	// == 연산자 오버로드
	bool operator==(const FWeaponItemInfo& Other) const
	{
		return ItemID == Other.ItemID && ItemGUID == Other.ItemGUID;
	}
};


USTRUCT(BlueprintType)
struct FInventoryInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FWeaponItemInfo WeaponItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bIsValid;
};


//추가해본 아이템 데이터
USTRUCT(BlueprintType) 
struct FAttackType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackType")
	int32 PhysicsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackType")
	int32 MagicAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackType")
	int32 Flame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackType")
	int32 Lightning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackType")
	int32 Divinity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackType")
	int32 Fatality;
};

//얘는 Enum
USTRUCT(BlueprintType)
struct FCorrectionStat
{
	GENERATED_BODY()

	//Enum으로 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	EWeaponStatType Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	EWeaponStatType Agility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	EWeaponStatType Intellect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	EWeaponStatType Faith;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	EWeaponStatType Mystery;
};

USTRUCT(BlueprintType)
struct FRequireStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	uint8 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	uint8 Agility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	uint8 Intellect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	uint8 Faith;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RequireStat")
	uint8 Mystery;
};

USTRUCT(BlueprintType)
struct FAttributeStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FScalableFloat BleedAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FScalableFloat BurnAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FScalableFloat DebuffAtkAmount;
	
};
//땅에 떨어진 아이템이 사용할 아이템 데이터
USTRUCT(BlueprintType)
struct FDroppedItemInfo: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EBaseInventoryType InventoryType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UPaperSprite* ItemIcon;
};

//무기의 강화 수치와 관련된 정보. 필요시 데이터 추가 or 삭제
USTRUCT(BlueprintType)
struct FWeaponStatisticsDataInfo: public FTableRowBase
{
	GENERATED_BODY()
	
	//고유스킬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponAbilityStatistics")
	FName IntrinsicSkillName;

	//소비FP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponAbilityStatistics")
	int32 CostFP;

	//공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponAbilityStatistics")
	FAttackType AttackPower;
	
	//경감률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponAbilityStatistics")
	FAttackType MitigationRate;
	
	//능력보정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponAbilityStatistics")
	FCorrectionStat CorrectionStat;
	
	//필요능력치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponAbilityStatistics")
	FRequireStat RequireStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponAbilityStatistics")
	FAttributeStruct AttributeStruct;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponAbilityStatistics")
	FAttributeLevels AttributeLevels;
};

//기존 WeaponData, 이거 이용해서 데이터 테이블 오른쪽 왼쪽 2개 생성
USTRUCT(BlueprintType)
struct FPlayerWeaponData: public FTableRowBase
{
	GENERATED_BODY()

	//무기 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FGameplayTag AbilityTag;

	//장착 시 부여할 능력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	TArray<FPlayerAbilitySet> WeaponAbilities;

	//특수 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FPlayerAbilitySet WeaponSpacialAbility;
	
	//장착 몽타주
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animation")
	TObjectPtr<UAnimMontage> EquipMontage;

	//탈착 몽타주
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animation")
	TObjectPtr<UAnimMontage> UnequipMontage;

	//애님레이어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	TSubclassOf<UPlayerLinkedAnimLayer> AnimLayerToLink;

	// 입력 매핑 컨텍스트 오브젝트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "WeaponData")
	TSubclassOf<APlayerWeaponBase> WeaponBP;
};

//소비UPROPERTY(아이템, 재료아이템 데이터 테이블에 적용할 구조체 작성. 설명이나 적용할 GE나


//필요시 추가가공을 통해 구조체 결합
USTRUCT(BlueprintType)
struct FWeaponAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseImpulse;
};

#pragma region SpellInfo

USTRUCT(BlueprintType)
struct FAshOfWarInfo: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War")
	FName SkillName;
	
	//공격형인지 패리형인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War")
	ESkillType SkillType;

	//타격형인지 프로젝타일형인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War",meta=(EditCondition="SkillType != ESkillType::None"))
	ESkillAdditionalType SkillAdditional;

	//소모FP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War",meta=(EditCondition="SkillType != ESkillType::None"))
	uint8 CostFP;

	//공격형인경우
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War",meta=(EditCondition="SkillType == ESkillType::Attack"))
	uint8 BaseImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War",meta=(EditCondition="SkillType == ESkillType::Attack"))
	uint8 BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo", meta=(EditCondition="SpellType == ESpellType::Attack"))
	FAttributeStruct AttributeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo", meta=(EditCondition="SpellType == ESpellType::Attack"))
	FAttributeLevels AttributeLevel;
	
	//사용할 몽타주(왼손)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War",meta=(EditCondition="SkillType != ESkillType::None"))
	UAnimMontage* WarMontageLeft;

	//사용할 몽타주(오른손)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War",meta=(EditCondition="SkillType != ESkillType::None"))
	UAnimMontage* WarMontageRight;

	//사용할 프로젝타일
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="War",meta=(EditCondition="SkillAdditional == ESkillAdditionalType::Projectile && SkillType != ESkillType::None"))
	TSubclassOf<ASpellEffect> Effect_BP;
	
};


USTRUCT(BlueprintType)
struct FInventorySpellInfo: public FTableRowBase
{
	GENERATED_BODY()

	//마술인지 기도인지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="InventorySpellInfo")
	ESpellCategory SpellCategory;

	//공격형인지 지원형인지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="InventorySpellInfo")
	ESpellType SpellType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="InventorySpellInfo")
	int32 CostFP;

	//무슨 마법인지 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="InventorySpellInfo")
	FText SpellContext;
	
};

USTRUCT(BlueprintType)
struct FPlayerSpellInfo: public FTableRowBase
{
	GENERATED_BODY()

	//마술인지 기도인지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo")
	ESpellCategory SpellCategory;

	//공격형인지 지원형인지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo")
	ESpellType SpellType;

	//소모비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo")
	int32 CostFP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo")
	UAnimMontage* LeftAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo")
	UAnimMontage* RightAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo")
	UPaperSprite* Icon;
	
	//계수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo")
	float Coefficient;

	//공격형 마법은 속성 타입이 있을거임.
	//makeAttributeHandle로 적용하려면 3가지 전부 있어야하는데 이걸 그냥 배열로 3개 지정 ㄱㄱ
	//레벨도 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo", meta=(EditCondition="SpellType == ESpellType::Attack"))
	FAttributeStruct AttributeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo", meta=(EditCondition="SpellType == ESpellType::Attack"))
	FAttributeLevels AttributeLevel;

	//공격형 마법이 장판형인지 투사체인지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerSpellInfo", meta=(EditCondition="SpellType == ESpellType::Attack"))
	EAttackForm AttackForm;

	//공격형일경우 소환할 액터
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "PlayerSpellInfo", meta=(EditCondition="SpellType == ESpellType::Attack"))
	TSubclassOf<ASpellEffect> SpellEffect;
	
	//지원형 마법이 버프인지 회복인지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo",meta=(EditCondition = "SpellType == ESpellType::Support"))
	ESupportForm SupportForm;

	//지원형 마법시 적용할 GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo",meta=(EditCondition = "SpellType == ESpellType::Support"))
	TSubclassOf<UGameplayEffect> SupportSpellEffect;

	//SetbyCallerTag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo",meta=(EditCondition = "SpellType == ESpellType::Support"))
	FGameplayTag SupportSetByCallerTag;

	//지원형 마법시 적용할 파티클
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo",meta=(EditCondition = "SpellType == ESpellType::Support"))
	UNiagaraSystem* Particle;
	
	//회복과 버프는 GE로 계산하되, 혹시모르니 커브테이블이나 그런곳에서 사용할 레벨값만 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpellInfo",meta=(EditCondition = "SpellType == ESpellType::Support"))
	uint8 SupportLevel;
	
};

USTRUCT(BlueprintType)
struct FSpellExecutionData
{
	GENERATED_BODY()

	//마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellExecutionData")
	bool bIsEnoughFP;
	
	//왼손 성공여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellExecutionData")
	bool bLeftHandCanExecute;

	//오른손 성공여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellExecutionData")
	bool bRightHandCanExecute;
	
	//실행타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellExecutionData")
	ESpellExecutionCategory SpellExecutionCategory;
};



#pragma endregion

#pragma region ConsumeItems
USTRUCT(BlueprintType)
struct FPotionInfo: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PotionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* Sprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage;

	//회복량 적용할 GE
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> HealingAmount;

	//회복 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Coefficient;

	//태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PotionTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnhancementLevel; // 강화 레벨
};

USTRUCT(BlueprintType)
struct FHorseInfo: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* Sprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerAbilitySet> Abilities;
};

USTRUCT(BlueprintType)
struct FConsumeInfo: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ConsumeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConsumeType ConsumeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* Icon;
};
#pragma endregion