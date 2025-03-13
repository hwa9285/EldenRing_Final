// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spell/SpellEffect.h"
#include "AreaSpellEffect.generated.h"

class UNiagaraComponent;
class USphereComponent;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API AAreaSpellEffect : public ASpellEffect
{
	GENERATED_BODY()

public:
	AAreaSpellEffect();
	
private:
	//오버랩 감지
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "AreaAttack")
	USphereComponent* CollisionSphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "AreaAttack")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), category = "AreaAttack")
	TArray<APawn*> OverlappedPawn;
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnSpellDestroyed(AActor* Actor);
	
};
