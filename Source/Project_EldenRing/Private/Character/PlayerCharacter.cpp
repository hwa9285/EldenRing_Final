// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Input/BaseInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "BaseGamePlayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/PlayerAttributeSet.h"
#include "Character/Horse.h"
#include "Components/Combat/HumanCombatComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Components/Stat/StatComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Controllers/HumanController.h"
#include "DataAssets/StartupData/DataAsset_StartupBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BaseDebugHelper.h"
#include "Interfaces/GraceInterface.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/NpcInteractInterface.h"
#include "Widget/WidgetBase.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//회전 사용 비활성화
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//스프링암 초기세팅
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	//카메라 초기세팅
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//케릭터움직임 초기세팅
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	//매시 세팅
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Assets/Player_Mesh/IdaFaber/Meshes/Girl/SK_MMO_CHAMPION_F_01.SK_MMO_CHAMPION_F_01"));

	if (MeshAsset.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.05f), FRotator(0, -90.0f, 0));

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(GetRootComponent());

	InteractCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//오버랩 채널 설정 필요
	InteractCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	PlayerAttribute = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("BaseAttributeSet"));
	
	HumanCombatComponent = CreateDefaultSubobject<UHumanCombatComponent>(TEXT("HumanCombatComponent"));
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryManagerComponent"));
	//InventoryManagerComponent->SetupAttachment(GetRootComponent());

	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	
	
	bIsBattle = false;
	bIsJump = false;
	bIsEquip = false;
	bIsCharging = false;
	bIsAttackRight = false;
	bIsAttackLeft = false;
	bIsRespawn = false;
	EquipLeftIndex = -1;
	EquipRightIndex = -1;

	OverlappingActors = TArray<AActor*>();
	CurrentInteractActor = nullptr;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InteractCollision)
	{
		InteractCollision->OnComponentBeginOverlap.AddDynamic(this,&APlayerCharacter::OnOverlapBegin);
		InteractCollision->OnComponentEndOverlap.AddDynamic(this,&APlayerCharacter::OnOverlapEnd);
	}
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Forget to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);
	
	UBaseInputComponent* BaseInputComponent= CastChecked<UBaseInputComponent>(PlayerInputComponent);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGamePlayTags::InputTag_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Move);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGamePlayTags::InputTag_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Look);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGamePlayTags::InputTag_Interact, ETriggerEvent::Started, this, &APlayerCharacter::Input_Interact);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGamePlayTags::InputTag_Menu, ETriggerEvent::Started, this, &APlayerCharacter::Input_Menu);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGamePlayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered,this, &APlayerCharacter::Input_SwitchTargetTriggered);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGamePlayTags::InputTag_SwitchTarget, ETriggerEvent::Completed,this, &APlayerCharacter::Input_SwitchTargetCompleted);
	
	
	BaseInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &APlayerCharacter::Input_AbilityInputPressed, &APlayerCharacter::Input_AbilityInputReleased);
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (!OverlappingActors.Contains(OtherActor))
		{
			if (OtherActor->Implements<UNpcInteractInterface>() || OtherActor->Implements<UItemPickupInterface>() || OtherActor->Implements<UGraceInterface>())
			{
				OverlappingActors.Add(OtherActor);
				if (!GetWorld()->GetTimerManager().IsTimerActive(InteractTimer))
				{
					GetWorld()->GetTimerManager().SetTimer(InteractTimer,this,&APlayerCharacter::CheckClosestActor,0.1f,true);
				}
			}
		}
	}
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (OverlappingActors.Contains(OtherActor))
		{
			OverlappingActors.Remove(OtherActor);
			if (CurrentInteractActor == OtherActor)
			{
				CurrentInteractActor = nullptr;
			}
		}
	}
	if (OverlappingActors.Num() == 0 && GetWorld()->GetTimerManager().IsTimerActive(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
		PlayerUIComponent->OnInteractActor.Broadcast(InteractString,false);
	}
}

void APlayerCharacter::CheckClosestActor()
{
	float MinDistanceSq = FMath::Square(1000.f);
	FVector PlayerLocation = GetActorLocation();

	AActor* TempActor = nullptr;
    
	for (AActor* Actor : OverlappingActors)
	{
		float DistanceSq = FVector::DistSquared(PlayerLocation, Actor->GetActorLocation());
		if (DistanceSq < MinDistanceSq)
		{
			MinDistanceSq = DistanceSq;
			TempActor = Actor;
		}
	}

	if (CurrentInteractActor != TempActor)
	{
		CurrentInteractActor = TempActor;
        
		if (CurrentInteractActor != nullptr)
		{
			if (CurrentInteractActor->Implements<UItemPickupInterface>())
			{
				InteractString = TEXT("아이템을 줍는다");
			}
			else if (CurrentInteractActor->Implements<UNpcInteractInterface>())
			{
				InteractString = TEXT("말을 건다");
			}
			else if (CurrentInteractActor->Implements<UGraceInterface>())
			{
				InteractString = TEXT("축복에서 쉰다");
			}

			PlayerUIComponent->OnInteractActor.Broadcast(InteractString, true);    
		}
	}
}

void APlayerCharacter::SetIsRiding(bool value,AHorse* CurrentHorse)
{
	bIsRidingHorse = value;
	if (bIsRidingHorse)
	{
		Horse = CurrentHorse;
	}
	else
	{
		Horse = nullptr;
	}
}


UPawnCombatComponent* APlayerCharacter::GetPawnCombatComponent() const
{
	return HumanCombatComponent;
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		if (!bIsRidingHorse)
		{
			AddMovementInput(ForwardDirection, MovementVector.Y);
		}
		else
		{
			Horse->AddMovementInput(ForwardDirection, MovementVector.Y);	
		}
		
	}
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		if (!bIsRidingHorse)
		{
			AddMovementInput(RightDirection, MovementVector.X);	
		}
		else
		{
			Horse->AddMovementInput(RightDirection, MovementVector.X);
		}
		
	}
}


void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void APlayerCharacter::Input_Interact()
{
	if (!CurrentInteractActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Interactable Actor"));
		return;
	}
	if (CurrentInteractActor->Implements<UItemPickupInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact Item"));
		InventoryManagerComponent->AddToInventory(CurrentInteractActor);
		//아이템 픽업 로직
	}
	else if (CurrentInteractActor->Implements<UNpcInteractInterface>())
	{
		INpcInteractInterface::Execute_InteractNPC(CurrentInteractActor,this);
		UE_LOG(LogTemp, Warning, TEXT("Interact NPC"));
	}
	else if (CurrentInteractActor->Implements<UGraceInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact Grace"));
		IGraceInterface::Execute_SaveData(CurrentInteractActor,this);
	}
}

void APlayerCharacter::Input_Menu()
{
	AHumanController* Player_Controller = Cast<AHumanController>(GetController<APlayerController>());
	ULocalPlayer* LocalPlayer = Player_Controller->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	
	if (MenuWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		MenuWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerHud->SetVisibility(ESlateVisibility::Hidden);
		PlayerUIComponent->OnVisibleWidget.Broadcast(true);
		MenuWidget->SetIsFocusable(true);
		
		UE_LOG(LogTemp, Warning, TEXT("Widget Visible"));

		FInputModeGameAndUI InputMode;
		Player_Controller->SetShowMouseCursor(true);
		Player_Controller->SetInputMode(InputMode);
		Subsystem->AddMappingContext(InputConfigDataAsset->UIMappingContext,1);
	}
	else
	{
		PlayerUIComponent->OnVisibleWidget.Broadcast(false);
		PlayerHud->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("Widget UnVisible"));

		FInputModeGameOnly InputMode;
		Player_Controller->SetShowMouseCursor(false);
		Player_Controller->SetInputMode(InputMode);
		Subsystem->RemoveMappingContext(InputConfigDataAsset->UIMappingContext);
	}
}

void APlayerCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void APlayerCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData EventData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X > 0.0f ? BaseGamePlayTags::Player_Event_SwitchTarget_Left : BaseGamePlayTags::Player_Event_SwitchTarget_Right,
		EventData
	);

	//Debug::Print(TEXT("SwitchDirection: " + SwitchDirection.ToString()));
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Hold_Shift))
	{
		bIsCharging = true;
		ChargingStartTime = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp,Warning,TEXT("Press Shift"));
	}
	else if (InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Spacebar) || InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Riding_Sprint))
	{
		bIsPressed = true;
		SpaceBarPressedTime = GetWorld()->GetTimeSeconds();
	}
	else if (InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Attack_RightHand))
	{
		bIsAttackRight = true;
	}
	else if (InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Attack_LeftHand))
	{
		bIsAttackLeft = true;
	}
	BaseAbilitySystemComponent->OnAbilityInputPressed(InputTag);
	//UE_LOG(LogTemp, Warning, TEXT("Press: %f"),SpaceBarPressedTime);
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Hold_Shift))
	{
		bIsCharging = false;
		ChargingEndTime = GetWorld()->GetTimeSeconds();
	}
	else if (InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Spacebar)|| InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Riding_Sprint))
	{
		bIsPressed = false;
		SpaceBarReleasedTime = GetWorld()->GetTimeSeconds();
	}
	else if (InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Attack_RightHand))
	{
		bIsAttackRight = false;
	}
	else if (InputTag.MatchesTagExact(BaseGamePlayTags::InputTag_Attack_LeftHand))
	{
		bIsAttackLeft = false;
	}
	BaseAbilitySystemComponent->OnAbilityInputReleased(InputTag);
	//UE_LOG(LogTemp, Warning, TEXT("Released: %f"),SpaceBarReleasedTime);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//이걸 따로 함수화해서 beginplay로 옮겨보기
	if (!StartupData.IsNull())
	{
		if (UDataAsset_StartupBase* LoadedData = StartupData.LoadSynchronous())
		{
			//Startup데이터가 Null이 아닌경우 StartupData는 동기화로드를 거쳐서 최종적으로 게임어빌리티시스템이 발동된다. GA_HeroSpawnAxe
			LoadedData->GiveToAbilitySystemComponent(GetBaseAbilitySystemComponent());
		}
	}
}

UPawnUIComponent* APlayerCharacter::GetPawnUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* APlayerCharacter::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

UStatComponent* APlayerCharacter::GetStatComponent() const
{
	return StatComponent;
}

UInventoryManagerComponent* APlayerCharacter::GetInventoryManagerComponent() const
{
	return InventoryManagerComponent;
}

int32 APlayerCharacter::GetEquipIndexFromType(ESpawnWeaponType Type)
{
	return (Type == ESpawnWeaponType::Left) ? EquipLeftIndex : EquipRightIndex;

}

void APlayerCharacter::SetEquipIndexFromType(ESpawnWeaponType Type, int32 value)
{
	Type == ESpawnWeaponType::Left ? EquipLeftIndex = value : EquipRightIndex = value;
}
