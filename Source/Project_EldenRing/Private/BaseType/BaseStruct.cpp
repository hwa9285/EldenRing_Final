// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseType/BaseStruct.h"

bool FPlayerAbilitySet::IsValid() const
{
 return InputTag.IsValid() && AbilityToGrant;;
}

