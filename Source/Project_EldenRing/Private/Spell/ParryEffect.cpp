// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell/ParryEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGamePlayTags.h"
#include "NiagaraComponent.h"
#include "Character/EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "Item/ProjectileBase.h"
#include "Item/Equipment/WeaponBase.h"

AParryEffect::AParryEffect()
{
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphereComponent"));
	SetRootComponent(CollisionSphereComponent);

	CollisionSphereComponent -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphereComponent -> SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	CollisionSphereComponent -> SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	CollisionSphereComponent -> SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SphereComponent"));
	NiagaraComponent->SetupAttachment(GetRootComponent());

	InitialLifeSpan = 2.5f;
}

void AParryEffect::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this,&AParryEffect::OnAreaBeginOverlap);
}

void AParryEffect::OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		//오버랩 된 액터가 적의 프로젝타일 공격이면
		if (AProjectileBase* Projectile = Cast<AProjectileBase>(OtherActor))
		{
			//삭제
			OtherActor->Destroy();
		}

		//오버랩 된 액터가 적의 무기이고
		if (AWeaponBase* EnemyWeapon= Cast<AWeaponBase>(OtherActor))
		{
			//플레이어가 패링중이면
			if (UBaseFunctionLibrary::NativeActorHasTag(GetOwner(),BaseGamePlayTags::Shared_Status_Parry))
			{
				FGameplayEventData EventData;
				EventData.Instigator = OtherActor->GetOwner();
				EventData.Target = GetOwner();

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), BaseGamePlayTags::Shared_Event_SuccessParry, EventData);

				//UE_LOG(LogTemp,Warning,TEXT("Effect Parry!"));
			}
		}
	}
}
