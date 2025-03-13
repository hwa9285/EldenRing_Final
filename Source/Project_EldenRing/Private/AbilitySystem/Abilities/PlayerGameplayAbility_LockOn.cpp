// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerGameplayAbility_LockOn.h"

#include "BaseFunctionLibrary.h"
#include "BaseGamePlayTags.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SizeBox.h"
#include "Controllers/HumanController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/WidgetBase.h"

void UPlayerGameplayAbility_LockOn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	InitTargetingMappingContext();
	TryLockOnTarget();
}

void UPlayerGameplayAbility_LockOn::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanUp();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGameplayAbility_LockOn::TryLockOnTarget()
{
	//타겟팅이 가능한 액터 탐색
	GetAvailableActorsToLock();

	//타겟팅 가능한 엑터가 없으면
	if (AvailableActorsToLock.IsEmpty())
	{
		FRotator CharacterRotation = GetPlayerCharacterFromActorInfo()->GetActorRotation();
		GetPlayerControllerFromActorInfo()->SetControlRotation(CharacterRotation);
		ResetTargetingMappingContext();
		CancelTargetingAbility();
		return;
	}

	//타겟팅 가능한 엑터들중 가장 가까운 타겟 가져오기
	CurrentTarget = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	if (CurrentTarget)
	{
		
		GetPlayerCharacterFromActorInfo()->SetTargetEnemy(CurrentTarget);
		UBaseFunctionLibrary::AddGameplayTagToActor(CurrentTarget,BaseGamePlayTags::Enemy_Target);
		DrawTargetingWidget();
		SetTargetingWidgetPosition();
	}
	else
	{
		ResetTargetingMappingContext();
		CancelTargetingAbility();
	}
}

void UPlayerGameplayAbility_LockOn::GetAvailableActorsToLock()
{
	TArray<FHitResult> HitResults;
	
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation() + GetPlayerCharacterFromActorInfo()->GetActorForwardVector() *TraceDistance,
		TraceBoxSize / 2.0f,
		GetPlayerCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		TraceChannel,
		false,
		TArray<AActor*>(),
		bShowDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		HitResults,
		true
	);

	for (const FHitResult& HitResult : HitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor != GetPlayerCharacterFromActorInfo())
			{
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

AActor* UPlayerGameplayAbility_LockOn::GetNearestTargetFromAvailableActors(const TArray<AActor*>& AvailableActors)
{
	//Distance는 대상과의 거리값이 나옴
	float Distance = 0.0f;
	return UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), AvailableActors, Distance);
}

void UPlayerGameplayAbility_LockOn::CancelTargetingAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UPlayerGameplayAbility_LockOn::CleanUp()
{
	if (CurrentTarget)
	{
		UBaseFunctionLibrary::RemoveGameplayTagFromActor(CurrentTarget,BaseGamePlayTags::Enemy_Target);	
	}
	ResetTargetingMappingContext();
	AvailableActorsToLock.Empty();
	CurrentTarget = nullptr;
	GetPlayerCharacterFromActorInfo()->SetTargetEnemy(CurrentTarget);
	if (TargetingWidget)
	{
		TargetingWidget->RemoveFromParent();
	}

	TargetingWidget = nullptr;
	TargetingWidgetSize = FVector2D::ZeroVector;
}

void UPlayerGameplayAbility_LockOn::DrawTargetingWidget()
{
	checkf(TargetingWidgetClass, TEXT("Forget to assigned  a valid widget class in Blueprint"));

	TargetingWidget = CreateWidget<UWidgetBase>(GetPlayerControllerFromActorInfo(), TargetingWidgetClass);
		
	check(TargetingWidget);
	
	TargetingWidget->AddToViewport();
}

void UPlayerGameplayAbility_LockOn::SetTargetingWidgetPosition()
{
	if (!TargetingWidget || !CurrentTarget)
	{
		CancelTargetingAbility();
		return;
	}

	FVector2D ScreenPosition;

	//적의 정 중앙에 표시되게 위치 계산
	UCapsuleComponent* Capsule = CurrentTarget->FindComponentByClass<UCapsuleComponent>();
	if (Capsule)
	{
		FVector CenterLocation = Capsule->GetComponentLocation();

		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetPlayerControllerFromActorInfo(),
		CenterLocation+FVector(0,0,30.f),
		ScreenPosition,
		true
	);
	}
	
	if (TargetingWidgetSize == FVector2D::ZeroVector)
	{
		TargetingWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetingWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetingWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}

	ScreenPosition -= (TargetingWidgetSize / 2.0f);

	TargetingWidget->SetPositionInViewport(ScreenPosition, false);
}

void UPlayerGameplayAbility_LockOn::InitTargetingMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(TargetingMappingContext, 3);
}

void UPlayerGameplayAbility_LockOn::ResetTargetingMappingContext()
{
	if (!GetPlayerControllerFromActorInfo())
	{
		return;
	}
	
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->RemoveMappingContext(TargetingMappingContext);

	UE_LOG(LogTemp, Warning, TEXT("Target mapping context:"));
}

void UPlayerGameplayAbility_LockOn::SwitchTarget(const FGameplayTag& SwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorOnRight;

	AActor* NewTargeting = nullptr;

	GetAvailableActorAroundTarget(ActorsOnLeft, ActorOnRight);

	if (SwitchDirectionTag == BaseGamePlayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargeting = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else
	{
		NewTargeting = GetNearestTargetFromAvailableActors(ActorOnRight);
	}

	if (NewTargeting)
	{
		UBaseFunctionLibrary::RemoveGameplayTagFromActor(CurrentTarget,BaseGamePlayTags::Enemy_Target);
		CurrentTarget = NewTargeting;
		UBaseFunctionLibrary::AddGameplayTagToActor(CurrentTarget,BaseGamePlayTags::Enemy_Target);
		GetPlayerCharacterFromActorInfo()->SetTargetEnemy(CurrentTarget);
	}
}

void UPlayerGameplayAbility_LockOn::GetAvailableActorAroundTarget(TArray<AActor*>& OutLeftActors,
	TArray<AActor*>& OutRightActors)
{
	if (!CurrentTarget || AvailableActorsToLock.IsEmpty())
	{
		GetPlayerCharacterFromActorInfo()->SetTargetEnemy(nullptr);
		CancelTargetingAbility();
		return;
	}

	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentTarget->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor: AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentTarget) continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();
		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		if (CrossResult.Z > 0.f)
		{
			OutRightActors.AddUnique(AvailableActor);
		}
		else
		{
			OutLeftActors.AddUnique(AvailableActor);
		}
	}
}

void UPlayerGameplayAbility_LockOn::OnTargetingTick(float DeltaTime)
{
	if (!CurrentTarget || UBaseFunctionLibrary::NativeActorHasTag(CurrentTarget, BaseGamePlayTags::Shared_Status_Dead)
		|| UBaseFunctionLibrary::NativeActorHasTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_Dead)
		)
	{
		CancelTargetingAbility();
		return;
	}
	//거리 넘어가면 타게팅 자동 종료
	if (FVector::Dist(GetPlayerCharacterFromActorInfo()->GetActorLocation() ,  CurrentTarget->GetActorLocation())>=TraceDistance)
	{
		CancelTargetingAbility();
		return;
	}

	SetTargetingWidgetPosition();

	// const bool bShouldOverrideRotation = !UBaseFunctionLibrary::NativeActorHasTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Player_)&&
	// 	!UBaseFunctionLibrary::NativeActorHasTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Player_Ability_Block);
	
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetPlayerCharacterFromActorInfo()->GetActorLocation(),
			CurrentTarget->GetActorLocation()
		);

	LookAtRot -= FRotator(TargetingCameraOffset, 0, 0);

	const FRotator CurrentControlRot = GetPlayerControllerFromActorInfo()->GetControlRotation();
	const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime,7.f );

	GetPlayerControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
	if (!GetPlayerCharacterFromActorInfo()->GetIsRiding())
	{
		GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.0f, LookAtRot.Yaw, 0.f));	
	}
}
