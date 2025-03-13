// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/WeaponBase.h"
#include "BaseFunctionLibrary.h"
#include "Components/BoxComponent.h"

#include "BaseDebugHelper.h"
#include "Item/Subsystem/InventoryGameInstanceSubsystem.h"

AWeaponBase::AWeaponBase()
{
    ItemCollsionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCollsionBox"));
    ItemCollsionBox->SetupAttachment(RootComponent);
    ItemCollsionBox->SetBoxExtent(FVector(20.0f));
    ItemCollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ItemCollsionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWeaponBase::OnItemCollisionBeginOverlap);
    ItemCollsionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &AWeaponBase::OnItemCollisionEndOverlap);




}

void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
    //UE_LOG(LogTemp, Error, TEXT("Weapon Row found for %s"), *WeaponName.ToString());
    
    UInventoryGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryGameInstanceSubsystem>();
    //WeaponRightData = Subsystem->GetWeaponRightRowData(ItemDTName);
    //WeaponLeftData = Subsystem->GetWeaponLeftRowData(ItemDTName);
}

void AWeaponBase::OnItemCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //Debug::Print(TEXT("++OnItemCollisionBeginOverlap++"));
    APawn* WeaponOwningPawn = GetInstigator<APawn>();

    check(WeaponOwningPawn);

    if (APawn* HitPawn = Cast<APawn>(OtherActor))
    {
        if (UBaseFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
        {
            //Debug::Print(TEXT("++TargetPawnHostile++"));
            OnWeaponHitTarget.ExecuteIfBound(OtherActor);
        }
        //Debug::Print(TEXT("++TargetPawn"));
    }
}

void AWeaponBase::OnItemCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APawn* WeaponOwningPawn = GetInstigator<APawn>();

    check(WeaponOwningPawn);

    if (APawn* HitPawn = Cast<APawn>(OtherActor))
    {
        if (UBaseFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
        {
            OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
        }
    }
}


