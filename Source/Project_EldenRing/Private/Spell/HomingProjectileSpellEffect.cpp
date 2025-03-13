// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell/HomingProjectileSpellEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGamePlayTags.h"
#include "NiagaraComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Controllers/HumanController.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AHomingProjectileSpellEffect::AHomingProjectileSpellEffect()
{
	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionSphereComponent"));
	SetRootComponent(CollisionBoxComponent);

	CollisionBoxComponent -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBoxComponent -> SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	CollisionBoxComponent -> SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	CollisionBoxComponent -> SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SphereComponent"));
	NiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->Velocity = FVector(1.0f, 0.0f, 0.0f);
	
	InitialLifeSpan = 4.f;
}



void AHomingProjectileSpellEffect::BeginPlay()
{
	Super::BeginPlay();

	CollisionBoxComponent->OnComponentHit.AddUniqueDynamic(this,&AHomingProjectileSpellEffect::OnProjectileHit);
	
	AHumanController* PlayerController = Cast<AHumanController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn());
	
	if (UBaseFunctionLibrary::NativeActorHasTag(Player,BaseGamePlayTags::Player_Status_Targeting))
	{
		ProjectileMovementComponent -> bIsHomingProjectile = true;
		ProjectileMovementComponent -> HomingTargetComponent = Player->GetTargetEnemy()->GetRootComponent();
	}
	else
	{
		ProjectileMovementComponent -> bIsHomingProjectile = false;
	}
	
}

void AHomingProjectileSpellEffect::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnSpawnProjectileHitFX(Hit.Location);

	//UE_LOG(LogTemp,Display,TEXT("On SpawnProjectileHitFX"));
	
	APawn* HitPawn = Cast<APawn>(OtherActor);

	if (HitPawn == nullptr || !UBaseFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
	{
		Destroy();
		return;
	}
	
	//GE Data Setting
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;


	//Apply projectile Damage
	HandleApplyProjectile(HitPawn, Data);

	Destroy();
}


