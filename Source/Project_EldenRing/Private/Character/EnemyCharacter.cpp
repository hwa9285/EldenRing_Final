// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "Components/Combat/EnemyCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAssets/StartupData/DataAsset_StartupBase.h"
#include "Engine/AssetManager.h"
#include "Components/WidgetComponent.h"

#include "BaseDebugHelper.h"
#include "BaseFunctionLibrary.h"
#include "MovieSceneTracksComponentTypes.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/EnemyAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "EntitySystem/MovieSceneComponentDebug.h"
#include "Item/SpawnItem/DroppedItemBase.h"
#include "Widget/WidgetBase.h"

AEnemyCharacter::AEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 360.0f, 0);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
	EnemyAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));
	
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());

	BossWeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BossWeaponCollisionBox"));
	BossWeaponCollisionBox-> SetupAttachment(GetMesh());
	BossWeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BossWeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&AEnemyCharacter::OnBodyCollisionBoxBeginOverlap);

	BossHeadCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BossHeadCollisionBox"));
	BossHeadCollisionBox-> SetupAttachment(GetMesh());
	BossHeadCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BossHeadCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&AEnemyCharacter::OnBodyCollisionBoxBeginOverlap);

	BossLeftArmCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BossLeftArmCollisionBox"));
	BossLeftArmCollisionBox-> SetupAttachment(GetMesh());
	BossLeftArmCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BossLeftArmCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&AEnemyCharacter::OnBodyCollisionBoxBeginOverlap);

	BossRightArmCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BossRightArmCollisionBox"));
	BossRightArmCollisionBox-> SetupAttachment(GetMesh());
	BossRightArmCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BossRightArmCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&AEnemyCharacter::OnBodyCollisionBoxBeginOverlap);
	
	BossLeftLegCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BossLeftLegCollisionBox"));
	BossLeftLegCollisionBox-> SetupAttachment(GetMesh());
	BossLeftLegCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BossLeftLegCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&AEnemyCharacter::OnBodyCollisionBoxBeginOverlap);

	BossRightLegCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BossRightLegCollisionBox"));
	BossRightLegCollisionBox-> SetupAttachment(GetMesh());
	BossRightLegCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BossRightLegCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&AEnemyCharacter::OnBodyCollisionBoxBeginOverlap);

	BossBodyCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BossBodyCollisionBox"));
	BossBodyCollisionBox-> SetupAttachment(GetMesh());
	BossBodyCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BossBodyCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&AEnemyCharacter::OnBodyCollisionBoxBeginOverlap);
	
	BossTailCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BossTailCollisionBox"));
	BossTailCollisionBox-> SetupAttachment(GetMesh());
	BossTailCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BossTailCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&AEnemyCharacter::OnBodyCollisionBoxBeginOverlap);
	
	UBaseFunctionLibrary::AddGameplayTagToActor(this, BaseGamePlayTags::Enemy_Status_Idle);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UWidgetBase* HealthWidget = Cast<UWidgetBase>(WidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreateWidget(this);
	}
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UBaseFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
		{
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
		}
	}
}

void AEnemyCharacter::SpawnItem()
{
	FHitResult Hit;
	FVector3d Start = this->GetActorLocation();
	FVector3d End = this->GetActorLocation() + FVector(0.0f,0.0f,-1000.0f);
	bool bIsSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Start, End,ECC_Visibility);
	if (bIsSuccess)
	{
		FTransform SpawnTransform(this->GetActorRotation(),Hit.Location );
		ADroppedItemBase* LocalItem = GetWorld()->SpawnActorDeferred<ADroppedItemBase>(DroppedItem, SpawnTransform);
		LocalItem->DroppedItemName = DroppedItemName;
		LocalItem->FinishSpawning(SpawnTransform);
	}
}

#if WITH_EDITOR
void AEnemyCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,BossWeaponCollisionBoxAttachBoneName))
	{
		BossWeaponCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,BossWeaponCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,BossHeadCollisionBoxAttachBoneName))
	{
		BossHeadCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,BossHeadCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,BossLeftArmCollisionBoxAttachBoneName))
	{
		BossLeftArmCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,BossLeftArmCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,BossRightArmCollisionBoxAttachBoneName))
	{
		BossRightArmCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,BossRightArmCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,BossLeftLegCollisionBoxAttachBoneName))
	{
		BossLeftLegCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,BossLeftLegCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,BossRightLegCollisionBoxAttachBoneName))
	{
		BossRightLegCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,BossRightLegCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,BossBodyCollisionBoxAttachBoneName))
	{
		BossBodyCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,BossBodyCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,BossTailCollisionBoxAttachBoneName))
	{
		BossTailCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,BossTailCollisionBoxAttachBoneName);
	}
}
#endif

UPawnCombatComponent* AEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AEnemyCharacter::InitEnemyStartUpData()
{
	if (StartupData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StartupData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UDataAsset_StartupBase* LoadedData = StartupData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(BaseAbilitySystemComponent);
					//UBaseFunctionLibrary::AddGameplayTagToActor(this, BaseGamePlayTags::Enemy_Status_Idle);
					//Debug::Print(TEXT("Loaded Enemy StartupData!"),FColor::Black);
				} 
			}
		)
	);
}
