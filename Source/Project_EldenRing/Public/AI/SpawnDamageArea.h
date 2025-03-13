// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnDamageArea.generated.h"

class USphereComponent;

UCLASS

()
class PROJECT_ELDENRING_API ASpawnDamageArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnDamageArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void  AmountDamage(AActor* OverlapActor ,const float LocalDamage);
private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float Damage;
};
