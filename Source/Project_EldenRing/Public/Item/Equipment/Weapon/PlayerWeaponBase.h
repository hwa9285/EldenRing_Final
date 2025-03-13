// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/WeaponBase.h"
#include "PlayerWeaponBase.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT_ELDENRING_API APlayerWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	

public:
	APlayerWeaponBase();
	
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category 
    //FDataTableRowHandle WeaponDataTableRowHandle;
	
    UFUNCTION(BlueprintCallable)
    void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles);

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAshOfWarSpecHandle(const FGameplayAbilitySpecHandle& Handle);
	
    UFUNCTION(BlueprintPure)
    TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

	UFUNCTION(BlueprintPure)
	FGameplayAbilitySpecHandle GetGrantedAshOfWarSpecHandle() const;
	
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
    //FWeaponData WeaponData;

	UFUNCTION(BlueprintPure)
	FName GetItemID(){return ItemID;}

	UFUNCTION(BlueprintPure)
	FName GetReinforceID(){return ReinforceID;}

	UFUNCTION(BlueprintCallable)
	void SetReinforceID(FName NewID){ReinforceID = NewID;};
	
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ItemID",meta=(AllowPrivateAccess=true))
	FName ItemID;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ItemID",meta=(AllowPrivateAccess=true))
	FName ReinforceID;
	
private:
    TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
	FGameplayAbilitySpecHandle GrantedAshOfWarSpecHandle;
};
