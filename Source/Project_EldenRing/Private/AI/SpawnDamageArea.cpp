// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SpawnDamageArea.h"

#include "AbilitySystem/PlayerAttributeSet.h"
#include "Character/PlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ASpawnDamageArea::ASpawnDamageArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(50.0f,true);

	InitialLifeSpan = 10.0f;
}

// Called when the game starts or when spawned
void ASpawnDamageArea::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this,&ASpawnDamageArea::OnBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this,&ASpawnDamageArea::OnEndOverlap);
}

void ASpawnDamageArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this , &ASpawnDamageArea::AmountDamage, OtherActor, Damage);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,1.0f,true);
}

void ASpawnDamageArea::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ASpawnDamageArea::AmountDamage(AActor* OverlapActor ,const float LocalDamage)
{
	//spechandle?
	
	
	/*const APlayerCharacter* HitCharacter = Cast<APlayerCharacter>(OverlapActor);
	HitCharacter->GetBaseAttributeSet()->SetDamageTaken(LocalDamage);*/
}

