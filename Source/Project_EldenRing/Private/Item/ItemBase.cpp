// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"
#include "Engine/DataTable.h"
// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneComponent;

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//RootComponent = ItemMeshComponent;
	ItemMeshComponent->SetupAttachment(SceneComponent);
	ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}





