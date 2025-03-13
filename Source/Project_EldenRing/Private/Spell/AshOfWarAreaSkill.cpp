// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell/AshOfWarAreaSkill.h"

#include "BaseFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Controllers/HumanController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAshOfWarAreaSkill::AAshOfWarAreaSkill()
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
	ProjectileMovementComponent->MaxSpeed = 500.0f;
	ProjectileMovementComponent->Velocity = FVector(1.0f, 0.0f, 0.0f);

	InitialLifeSpan = 2.5f;
}

void AAshOfWarAreaSkill::BeginPlay()
{
	Super::BeginPlay();
	CollisionBoxComponent->OnComponentHit.AddUniqueDynamic(this,&AAshOfWarAreaSkill::OnAreaHit);

	AHumanController* PlayerController = Cast<AHumanController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn());
	
	if (UBaseFunctionLibrary::NativeActorHasTag(Player,BaseGamePlayTags::Player_Status_Targeting))
	{
		ProjectileMovementComponent -> HomingTargetComponent = Player->GetTargetEnemy()->GetRootComponent();
		ProjectileMovementComponent -> bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingAccelerationMagnitude = 2000.f;
		UE_LOG(LogTemp,Warning,TEXT("Homing Projectile, HommingTarget: %s"),*Player->GetTargetEnemy()->GetRootComponent()->GetName());
	}
	else
	{
		ProjectileMovementComponent -> HomingTargetComponent = nullptr;
		ProjectileMovementComponent -> bIsHomingProjectile = false;
	}
}

void AAshOfWarAreaSkill::OnAreaHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* OverlapPawn = Cast<APawn>(OtherActor);

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
