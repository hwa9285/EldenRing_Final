// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell/AreaSpellEffect.h"

#include "BaseFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/SphereComponent.h"

AAreaSpellEffect::AAreaSpellEffect()
{
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphereComponent"));
	SetRootComponent(CollisionSphereComponent);

	CollisionSphereComponent -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphereComponent -> SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	CollisionSphereComponent -> SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	CollisionSphereComponent -> SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SphereComponent"));
	NiagaraComponent->SetupAttachment(GetRootComponent());

	InitialLifeSpan = 2.5f;

}

void AAreaSpellEffect::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this,&AAreaSpellEffect::OnAreaBeginOverlap);
	OnDestroyed.AddUniqueDynamic(this,&AAreaSpellEffect::OnSpellDestroyed);
}

void AAreaSpellEffect::OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* OverlapPawn = Cast<APawn>(OtherActor);
	OverlappedPawn.Add(OverlapPawn);

	UE_LOG(LogTemp,Log,TEXT("OnAreaBeginOverlap"));
}

void AAreaSpellEffect::OnSpellDestroyed(AActor* Actor)
{
	//GE Data Setting
	FGameplayEventData Data;
	Data.Instigator = this;
	
	for (auto& Enemy : OverlappedPawn)
	{
		if (Enemy)
		{
			BP_OnSpawnProjectileHitFX(Enemy->GetActorLocation());
			Data.Target = Enemy;
			HandleApplyProjectile(Enemy, Data);	
		}
	}
	
}

