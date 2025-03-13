// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/Weapon/PlayerWeaponBase.h"

APlayerWeaponBase::APlayerWeaponBase()
{

}

void APlayerWeaponBase::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
{
    GrantedAbilitySpecHandles = SpecHandles;

    //WeaponData =  WeaponDataTableRowHandle.GetRow<FWeaponData>(WeaponDataTableRowHandle)
}

void APlayerWeaponBase::AssignGrantedAshOfWarSpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
    GrantedAshOfWarSpecHandle = Handle;
}

TArray<FGameplayAbilitySpecHandle> APlayerWeaponBase::GetGrantedAbilitySpecHandles() const
{
    return GrantedAbilitySpecHandles;
}

FGameplayAbilitySpecHandle APlayerWeaponBase::GetGrantedAshOfWarSpecHandle() const
{
    return GrantedAshOfWarSpecHandle;
}
