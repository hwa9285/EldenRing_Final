// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "BaseType/BaseEnum.h"
#include "GameFramework/Actor.h"
#include "SpellEffect.generated.h"

struct FGameplayEventData;

UCLASS()
class PROJECT_ELDENRING_API ASpellEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellEffect();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnHit;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);
	
	void HandleApplyProjectile(APawn* HitPawn, FGameplayEventData& Payload);
	
	UPROPERTY(BlueprintReadOnly, Category="DamageHandle", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle ProjectileDamageSpecHandle;

	UPROPERTY(BlueprintReadOnly, Category="AttributeHandle", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle AttributeSpecHandle;
	
	
};
