// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SpawnItem/DroppedItemBase.h"
#include "Components/BoxComponent.h"

// Sets default values
ADroppedItemBase::ADroppedItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ItemCollsionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DropedItemCollsionBox"));
	ItemCollsionBox->SetupAttachment(RootComponent);
	ItemCollsionBox->SetBoxExtent(FVector(20.0f));

	ItemCollsionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemCollsionBox->SetCollisionObjectType(ECC_GameTraceChannel1); // 'ItemCollision'의 Object Type
	ItemCollsionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemCollsionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // 아이템 콜리전 채널에만 반응
}

// Called when the game starts or when spawned
void ADroppedItemBase::BeginPlay()
{
	Super::BeginPlay();
}

FName ADroppedItemBase::GetName_Implementation() const
{
	return DroppedItemName;
}

FItemInfos ADroppedItemBase::GetItemInfos_Implementation() const
{
	FItemInfos itemInfos;
	itemInfos.ItemID = DroppedItemName;
	itemInfos.ItemTypeID = WeaponStatisticsName;
	itemInfos.Quantity = DroppedItemAmount;
	return itemInfos;
}










