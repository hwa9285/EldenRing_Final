// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SpawnItem/ItemSpawnBase.h"
#include "BaseType/BaseStruct.h"
#include "Character/PlayerCharacter.h"
#include "Item/Equipment/Weapon/PlayerWeaponBase.h"

AItemSpawnBase::AItemSpawnBase()
{
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(SceneComponent);
}

void AItemSpawnBase::BeginPlay()
{
	Super::BeginPlay();

	ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
