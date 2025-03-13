// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseType/BaseEnum.h"
#include "BaseType/BaseStruct.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.generated.h"


class AHorse;
class APlayerWeaponBase;
class UWidgetBase;
class UPlayerAttributeSet;
class UPlayerUIComponent;
class UGameplayEffect;
enum class EStatTypes : uint8;
class UStatComponent;
struct FGameplayTag;
struct FInputActionValue;
class UDataAsset_InputConfig;
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UHumanCombatComponent;
class UInventoryManagerComponent;


/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void LoadData();
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void LoadInventory(FSaveInventory SaveData);

	//UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	//void LoadStat(FSaveStat SaveStatData);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void LoadAttribute(FSaveAttribute SaveAttribute);
	
private:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = InteractItemCollision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UHumanCombatComponent* HumanCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UStatComponent* StatComponent;
	
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UPlayerAttributeSet* PlayerAttribute;

	FVector2D SwitchDirection = FVector2D::ZeroVector;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Interact();
	void Input_Menu();
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);
	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);

	virtual void Landed(const FHitResult& Hit) override;
	

	
#pragma endregion

#pragma region Horse
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	AHorse* Horse;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AHorse* GetHorse(){return Horse;}
	
#pragma endregion
	
private:
#pragma region Interact

	TArray<AActor*> OverlappingActors;
	
	AActor* CurrentInteractActor;

	FTimerHandle InteractTimer;

	FString InteractString;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void CheckClosestActor();
	
#pragma endregion
	
#pragma region Player State
	bool bIsJump;
	bool bIsMoving;
	bool bIsBattle;
	bool bIsEquip;
	bool bIsRespawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Horse, meta = (AllowPrivateAccess = "true"))
	bool bIsCanDoubleJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Horse, meta = (AllowPrivateAccess = "true"))
	bool bIsRidingHorse;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target, meta = (AllowPrivateAccess = "true"))
	AActor* TargetEnemy;

	int32 NumberDetected = 0;
	
	UFUNCTION(BlueprintCallable,Category = Equip, meta = (AllowPrivateAccess = "true"))
	void SetIsEquip(bool value){ bIsEquip = value;};
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = Equip, meta = (AllowPrivateAccess = "true"))
	bool GetIsEquip(){return bIsEquip;};

	UFUNCTION(BlueprintCallable,Category = Equip, meta = (AllowPrivateAccess = "true"))
	void SetIsRiding(bool value,AHorse* CurrentHorse);

	UFUNCTION(BlueprintCallable,Category = Equip, meta = (AllowPrivateAccess = "true"))
	void SetIsCanDoubleJump(bool Value){bIsCanDoubleJump = Value;}

public:
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = Equip)
	bool GetIsRiding(){return bIsRidingHorse;};
	
	void SetIsBattle(bool Value){bIsBattle = Value;};
	
	void IncreaseNumberDetected(){NumberDetected++; UE_LOG(LogTemp, Warning, TEXT("IncreaseNumberDetected: %d"), NumberDetected);};

	UFUNCTION(BlueprintCallable,Category = Battle)
	void DecreaseNumberDetected(){if (NumberDetected > 0 ) NumberDetected--;};

	UFUNCTION(BlueprintCallable,Category = Target)
	void SetTargetEnemy(AActor* CurrentTarget){TargetEnemy = CurrentTarget;};

	UFUNCTION(BlueprintCallable,Category = Target)
	AActor* GetTargetEnemy(){return TargetEnemy;}
	
#pragma endregion

#pragma region Input SpaceBar Time
	// Time tracking variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hold, meta = (AllowPrivateAccess = "true"))
	float SpaceBarPressedTime;   // Key pressed timestamp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hold, meta = (AllowPrivateAccess = "true"))
	float SpaceBarReleasedTime; // Key released timestamp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hold, meta = (AllowPrivateAccess = "true"))
	bool bIsPressed = false;
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = HoldingTime)
	float GetSpaceBarHoldingTime() { return SpaceBarReleasedTime - SpaceBarPressedTime; };

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = Holding)
	bool GetIsPressed(){return bIsPressed;};
	// Input Handlers
	
#pragma endregion

#pragma region PlayerAttack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hold, meta = (AllowPrivateAccess = "true"))
	float ChargingStartTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hold, meta = (AllowPrivateAccess = "true"))
	float ChargingEndTime;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=HoldShift, meta = (AllowPrivateAccess = "true"))
	bool bIsCharging;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=HoldAttack, meta = (AllowPrivateAccess = "true"))
	bool bIsAttackRight;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=HoldAttack, meta = (AllowPrivateAccess = "true"))
	bool bIsAttackLeft;
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = Holding)
	bool GetIsCharging(){return bIsCharging;};

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = Holding)
	bool GetIsAttackRight(){return bIsAttackRight;};

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = Holding)
	bool GetIsAttackLeft(){return bIsAttackLeft;};

#pragma endregion

#pragma region Parry
	AActor* ParryTarget;

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = Parry)
	AActor* GetParryTarget(){return ParryTarget;}

	UFUNCTION(BlueprintCallable,Category = Parry)
	void SetParryTarget(AActor* CurrentParry){ParryTarget = CurrentParry;};
	
#pragma endregion 
	

#pragma region UIComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UPlayerUIComponent* PlayerUIComponent;
	
#pragma endregion

#pragma region EquipIndex and EquipUnequiphandle
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EquipIndex, meta = (AllowPrivateAccess = "true"))
	int32 EquipRightIndex;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EquipIndex, meta = (AllowPrivateAccess = "true"))
	int32 EquipLeftIndex;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EquipWeapon)
	APlayerWeaponBase* CurrentEquipLeftWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EquipWeapon)
	APlayerWeaponBase* CurrentEquipRightWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EquipWeapon)
	FName CurrentActivateAshOfWarWeaponID;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EquipWeapon)
	ESpawnWeaponType CurrentActivateAshOfWarHandType;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EquipWeapon)
	FWeaponStatisticsDataInfo CurrentRightEquipStatisticsDataInfo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EquipWeapon)
	FWeaponStatisticsDataInfo CurrentLeftEquipStatisticsDataInfo;
	
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category = EquipHandle)
	void EquipHandle(ESpawnWeaponType Type,APlayerWeaponBase *CurrentWeapon);

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category = UnequipHandle)
	void UnequipHandle(ESpawnWeaponType Type,APlayerWeaponBase *CurrentWeapon);
	
#pragma endregion

#pragma region PlayerHud And Menu
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Hud, meta = (AllowPrivateAccess = "true"))
	UWidgetBase* PlayerHud;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Hud, meta = (AllowPrivateAccess = "true"))
	UWidgetBase* MenuWidget;
	
#pragma endregion
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	virtual UStatComponent* GetStatComponent() const override;
	virtual UInventoryManagerComponent* GetInventoryManagerComponent() const override;

public:
	FORCEINLINE UHumanCombatComponent* GetHumanCombatComponent() const { return HumanCombatComponent; }
	FORCEINLINE UPlayerAttributeSet* GetBaseAttributeSet() const { return PlayerAttribute; }
	FORCEINLINE UStatComponent* GetPlayerStatComponent() const{return StatComponent;}
	UCameraComponent* GetFollowCamera()const {return FollowCamera;};
	UInventoryManagerComponent* GetInventoryManager()const {return InventoryManagerComponent;}
	void SetInventoryManager(UInventoryManagerComponent* NewInventoryManager) {InventoryManagerComponent = NewInventoryManager;}
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = EquipIndex)
	int32 GetEquipIndexFromType(ESpawnWeaponType Type);

	UFUNCTION(BlueprintCallable, Category = EquipIndex)
	void SetEquipIndexFromType(ESpawnWeaponType Type, int32 value);
	
};


inline void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (bIsPressed)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300;
	}
}

