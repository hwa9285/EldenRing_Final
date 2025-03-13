// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Spell/SpellEffect.h"
#include "HomingProjectileSpellEffect.generated.h"

struct FGameplayEventData;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API AHomingProjectileSpellEffect : public ASpellEffect
{
	GENERATED_BODY()

public:
	AHomingProjectileSpellEffect();

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "AreaAttack")
	UBoxComponent* CollisionBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "AreaAttack")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	

	UPROPERTY(BlueprintReadOnly, Category="Projectile", meta = (ExposeOnSpawn = true))
	float ProjectileSpeed = 1000.0f;
	

};
