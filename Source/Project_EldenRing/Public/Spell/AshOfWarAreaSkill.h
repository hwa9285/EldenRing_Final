// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spell/SpellEffect.h"
#include "AshOfWarAreaSkill.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API AAshOfWarAreaSkill : public ASpellEffect
{
	GENERATED_BODY()

public:
	AAshOfWarAreaSkill();

private:
	//오버랩 감지
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "AshOfWarAreaSkill")
	UBoxComponent* CollisionBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "AshOfWarAreaSkill")
	UNiagaraComponent* NiagaraComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "AshOfWarAreaSkill")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
protected:
	virtual void BeginPlay() override;

	//UFUNCTION()
	//FVector TargetLocation;
	
	UFUNCTION()
	virtual void OnAreaHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadOnly, Category="AshOfWarAreaSkill", meta = (ExposeOnSpawn = true))
	float ProjectileSpeed = 300.0f;
};
