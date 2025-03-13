// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGamePlayTags.h"
#include "NiagaraComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AI/SpawnDamageArea.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>("CollisionBoxComponent");
	SetRootComponent(CollisionBoxComponent);
	CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	CollisionBoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionBoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBoxComponent->SetSimulatePhysics(true);
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->Velocity = FVector(1.0f, 0.0f, 0.0f);
	ProjectileMovementComponent->ProjectileGravityScale = 1.0f;

	InitialLifeSpan = 11.0f;
}


void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionBoxComponent->OnComponentHit.AddUniqueDynamic(this,&AProjectileBase::OnProjectileHit);
	CollisionBoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this,&AProjectileBase::OnProjectileBeginOverlap);
	
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(ProjectileSpeed, 0.0f, 0.0f));
	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		CollisionBoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	}
}

void AProjectileBase::SpawnDamageArea(FVector3d SpawnLocation)
{
	//UE_LOG(LogTemp, Warning, TEXT("SpawnMagmaLocation : X - %f, Y - %f, Z- %f"),SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z)
	GetWorld()->SpawnActor<ASpawnDamageArea>(DamageAreaClass,SpawnLocation,FRotator3d(0.0f,0.0f,0.0f));
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnProjectileHit"));
	SpawnDamageArea(Hit.Location);
	
	BP_OnSpawnProjectileHitFX(Hit.Location);
	
	APawn* HitPawn = Cast<APawn>(OtherActor);

	if (HitPawn == nullptr || !UBaseFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
	{
		Destroy();
		return;
	}

	bool bIsValidBlock = false;

	//player가 Block중인지 판별
	const bool bIsPlayerBlocking = UBaseFunctionLibrary::NativeActorHasTag(HitPawn,BaseGamePlayTags::Player_Status_Block);

	if (bIsPlayerBlocking)
	{
		bIsValidBlock = UBaseFunctionLibrary::IsValidBlock(this, HitPawn);
	}

	//GE Data Setting
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;

	if (bIsValidBlock)
	{
		//Send to HitPawn (SuccessBlock)
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			BaseGamePlayTags::Player_Event_SuccessBlock,
			Data
		);
	}
	else
	{
		//Apply projectile Damage
		HandleApplyProjectile(HitPawn, Data);
	}

	Destroy();
}

void AProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AProjectileBase::HandleApplyProjectile(APawn* HitPawn, FGameplayEventData& Payload)
{
	checkf(ProjectileDamageSpecHandle.IsValid(), TEXT("ProjectileDamageSpecHandle must be valid"));
	checkf(ProjectileBurnAttributeSpecHandle.IsValid(), TEXT("ProjectileBurnAttributeSpecHandle must be valid"));
	
	const bool bWasApplied = UBaseFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), HitPawn, ProjectileDamageSpecHandle);
	UBaseFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), HitPawn, ProjectileBurnAttributeSpecHandle);
	
	if (bWasApplied)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			BaseGamePlayTags::Shared_Event_HitReact,
			Payload
		);
	}
}


