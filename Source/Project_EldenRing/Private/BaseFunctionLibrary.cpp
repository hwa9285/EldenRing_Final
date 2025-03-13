// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "BaseType/BaseEnum.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseDebugHelper.h"
#include "Character/Horse.h"

UBaseAbilitySystemComponent* UBaseFunctionLibrary::NativeGetAbilitySystemComponentFromActor(AActor* Actor)
{
	check(Actor);
	
	return CastChecked<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

void UBaseFunctionLibrary::AddGameplayTagToActor(AActor* Actor, const FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetAbilitySystemComponentFromActor(Actor);
	if (ASC->HasMatchingGameplayTag(Tag) == false)
	{
		ASC->AddLooseGameplayTag(Tag);
	}
}

void UBaseFunctionLibrary::RemoveGameplayTagFromActor(AActor* Actor, const FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetAbilitySystemComponentFromActor(Actor);
	if (ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->RemoveLooseGameplayTag(Tag);
	}
}

bool UBaseFunctionLibrary::NativeActorHasTag(AActor* Actor, const FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetAbilitySystemComponentFromActor(Actor);
	
	return ASC->HasMatchingGameplayTag(Tag);
}

void UBaseFunctionLibrary::BP_HasTag(AActor* Actor, const FGameplayTag Tag, EBaseConfirmType& OutType)
{
	OutType = NativeActorHasTag(Actor, Tag) ? EBaseConfirmType::Yes : EBaseConfirmType::No;
}

UPawnCombatComponent* UBaseFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* Actor)
{
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(Actor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	
	return nullptr;
}

UPawnCombatComponent* UBaseFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* Actor, EBaseValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(Actor);
	OutValidType = CombatComponent ? EBaseValidType::Valid : EBaseValidType::Invalid;

	return CombatComponent;
}

bool UBaseFunctionLibrary::IsTargetPawnHostile(const APawn* OwningPawn, const APawn* TargetPawn)
{
	check(OwningPawn && TargetPawn);

	IGenericTeamAgentInterface* OwningTeamAgent = Cast<IGenericTeamAgentInterface>(OwningPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (OwningTeamAgent && TargetTeamAgent)
	{
		UE_LOG(LogTemp, Log, TEXT("TargetTeamID: %d"), TargetTeamAgent->GetGenericTeamId().GetId());
		return OwningTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	
	return false;
}

FGameplayTag UBaseFunctionLibrary::ComputeHitReactDirectionTag(AActor* Attacker, AActor* HitActor,
	float& OutAngleDiffrecence)
{
	check(Attacker && HitActor);

	const FVector HitForward = HitActor->GetActorForwardVector();
	const FVector HitToAttackerNormalized = (Attacker->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal();
	
	//내적을->각도판단
	const float DotResult = FVector::DotProduct(HitForward, HitToAttackerNormalized);
	OutAngleDiffrecence = UKismetMathLibrary::DegAcos(DotResult);

	//외적->좌우판단
	const FVector CrossResult = FVector::CrossProduct(HitForward, HitToAttackerNormalized);
	
	if (CrossResult.Z < 0.f)
	{
		OutAngleDiffrecence *= -1.f;
	}

	if (OutAngleDiffrecence >= -15.f && OutAngleDiffrecence <= 15.f)
	{
		return BaseGamePlayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDiffrecence < -15.f && OutAngleDiffrecence > -165.0f)
	{
		return BaseGamePlayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDiffrecence > 15.f && OutAngleDiffrecence < 165.0f)
	{
		return BaseGamePlayTags::Shared_Status_HitReact_Right;
	}
	else if (OutAngleDiffrecence <= -165.0f || OutAngleDiffrecence >= 165.0f)
	{
		return BaseGamePlayTags::Shared_Status_HitReact_Back;
	}
	
	return BaseGamePlayTags::Shared_Status_HitReact_Front;
}

EDirectionType UBaseFunctionLibrary::FindDirectionTargetActor(AActor* TargetActor, AActor* SelfActor)
{
	EDirectionType Direction;

	if (!TargetActor || !SelfActor)
	{
		return EDirectionType::None;
	}
	
	const FVector SelfLocation= SelfActor->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();
	const FVector HitVector = (TargetLocation - SelfLocation).GetSafeNormal();

	//DotProduct
	const float DotValue = FVector::DotProduct(SelfActor->GetActorForwardVector(),HitVector);
	//float Radian = FMath::Acos(DotValue);
	//float Angle = FMath::RadiansToDegrees(Radian);

	/*
	FString MSG = TEXT("DotProduct Angle : ");
	
	MSG += FString::SanitizeFloat(Angle);
	MSG += TEXT("\nRadian : ");
	MSG += FString::SanitizeFloat(Radian);
	MSG += TEXT("\nDotValue : ");
	MSG += FString::SanitizeFloat(DotValue);

	Debug::Print(MSG,FColor::Cyan);*/

	//CrossProduct
	const FVector CrossValue = FVector::CrossProduct(SelfActor->GetActorForwardVector(),HitVector);
	if (DotValue > 0.5)
	{
		//Debug::Print(TEXT("Front"),FColor::Red);
		Direction = EDirectionType::Front;
	}
	else if (DotValue < -0.5)
	{
		//Debug::Print(TEXT("Back"),FColor::Red);
		Direction = EDirectionType::Back;
	}
	else if (CrossValue.Z < 0)
	{
		//Debug::Print(TEXT("Left"),FColor::Red);
		Direction = EDirectionType::Left;
	}
	else
	{
		//Debug::Print(TEXT("Right"),FColor::Red);
		Direction = EDirectionType::Right;
	}
	return Direction;
}

bool UBaseFunctionLibrary::IsValidBlock(const AActor* Attacker, const AActor* Defender)
{
	check(Attacker && Defender);

	//내적, 마주보고 있으면 -1에 가까운 값을 반환하고, 뒤를 보면 1에 가까운 값을 반환
	const float DotResult = FVector::DotProduct(Attacker->GetActorForwardVector(), Defender->GetActorForwardVector());

	/*
	const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"),DotResult, DotResult <0.1f ? TEXT("Valid Block") : TEXT("InValid Block"));
	Debug::Print(DebugString, DotResult < 0.1f ? FColor::Green : FColor::Red );
	*/
	
	return DotResult < 0.1f;
}

bool UBaseFunctionLibrary::IsValidFront(const AActor* Attacker, const AActor* Defender)
{
	check(Attacker && Defender);

	FVector EnemyForward = Defender->GetActorForwardVector();
	FVector DirectionToPlayer = (Attacker->GetActorLocation() - Defender->GetActorLocation()).GetSafeNormal();

	// 45도 임계값에 해당하는 코사인 값 계산
	float AngleThreshold = FMath::Cos(FMath::DegreesToRadians(45.f));

	// 내적 계산
	float DotResult = FVector::DotProduct(EnemyForward, DirectionToPlayer);

	if (DotResult > AngleThreshold)
	{
		return true;
	}
	else
	{
		return false;// 플레이어가 적의 정면에 없으므로 처형공격 불가 처리
	}
	
}

bool UBaseFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* Instigator, AActor* TargetActor,
                                                                      const FGameplayEffectSpecHandle& OutSpecHandle)
{
	UBaseAbilitySystemComponent* SourceASC = NativeGetAbilitySystemComponentFromActor(Instigator);
	UBaseAbilitySystemComponent* TargetASC = NativeGetAbilitySystemComponentFromActor(TargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*OutSpecHandle.Data,TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}
