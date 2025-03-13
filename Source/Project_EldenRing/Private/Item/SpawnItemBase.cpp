// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SpawnItemBase.h"

// Sets default values
ASpawnItemBase::ASpawnItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneComponent;

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//RootComponent = ItemMeshComponent;
	ItemMeshComponent->SetupAttachment(SceneComponent);

	ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemMeshComponent->SetCollisionObjectType(ECC_GameTraceChannel1); // 'ItemCollision'의 Object Type
	ItemMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // 아이템 콜리전 채널에만 반응
}

FName ASpawnItemBase::GetName_Implementation() const
{
	return ItemDTName;
}



