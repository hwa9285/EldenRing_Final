// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "BaseType/BaseEnum.h"
#include "ProjectileBase.generated.h"

class ASpawnDamageArea;
struct FGameplayEventData;
class UProjectileMovementComponent;
class UBoxComponent;
class UNiagaraComponent;
class USkeletalMeshComponent;

UCLASS()
class PROJECT_ELDENRING_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "Projectile")	
	UBoxComponent* CollisionBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "Projectile")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "Projectile")
	UStaticMeshComponent* ProjectileMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy;
	
	virtual void BeginPlay() override;
	
protected:
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);

	UPROPERTY(BlueprintReadOnly, Category="Projectile", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle ProjectileDamageSpecHandle;

	UPROPERTY(BlueprintReadOnly, Category="Projectile", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle ProjectileBurnAttributeSpecHandle;
	
	UPROPERTY(BlueprintReadOnly, Category="Projectile", meta = (ExposeOnSpawn = true))
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASpawnDamageArea> DamageAreaClass;
	
	void SpawnDamageArea(FVector3d SpawnLocation);
	
private:
	void HandleApplyProjectile(APawn* HitPawn, FGameplayEventData& Payload);
};
