// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/EquipmentBase.h"
#include "GameplayAbilitySpecHandle.h"
#include "BaseType/BaseStruct.h"
#include "WeaponBase.generated.h"

class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT_ELDENRING_API AWeaponBase : public AEquipmentBase
{
	GENERATED_BODY()

public:

    AWeaponBase();
    
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponData")
    FWeaponData WeaponRightData;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponData")
    FWeaponData WeaponLeftData;
    
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponName")
    //FName WeaponName;
    
    FOnTargetInteractedDelegate OnWeaponHitTarget;
    FOnTargetInteractedDelegate OnWeaponPulledFromTarget;
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    UBoxComponent* ItemCollsionBox;

    UFUNCTION()
    virtual void BeginPlay() override;
    
    UFUNCTION()
    virtual void OnItemCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnItemCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    FORCEINLINE UBoxComponent* GetItemCollsionBox() const { return ItemCollsionBox; }
};
