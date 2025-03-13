// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask_PlayMonatageCustom.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"


UAbilityTask_PlayMonatageCustom* UAbilityTask_PlayMonatageCustom::PlayMontageAndWaitCustom(
	UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* InMontageToPlay, float InRate,
	FName InStartSection, bool InStopWhenAbilityEnds, float InAnimRootMotionTranslationScale,
	USkeletalMeshComponent* InCustomSkeletalMesh)
{
	UAbilityTask_PlayMonatageCustom* MyObj = NewAbilityTask<UAbilityTask_PlayMonatageCustom>(OwningAbility, TaskInstanceName);
	
	// 부모 클래스의 변수에 In 값 할당
	MyObj->MontageToPlay = InMontageToPlay;
	MyObj->Rate = InRate;
	MyObj->StartSection = InStartSection;
	MyObj->bStopWhenAbilityEnds = InStopWhenAbilityEnds;
	MyObj->AnimRootMotionTranslationScale = InAnimRootMotionTranslationScale;
	
	// 추가: 커스텀 SkeletalMesh 설정
	MyObj->CustomSkeletalMesh = InCustomSkeletalMesh;
	
	return MyObj;
}

void UAbilityTask_PlayMonatageCustom::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;

	// AbilitySystemComponent가 유효한지 확인
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		// CustomSkeletalMesh가 있으면 이를 사용, 없으면 기본 ActorInfo의 AnimInstance를 사용
		UAnimInstance* AnimInstance = nullptr;
		if (CustomSkeletalMesh)
		{
			AnimInstance = CustomSkeletalMesh->GetAnimInstance();
			UE_LOG(LogTemp,Warning,TEXT("AnimInstance: %s"),*AnimInstance->GetName());
		}
		else
		{
			const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
			AnimInstance = ActorInfo->GetAnimInstance();
		}

		if (AnimInstance != nullptr)
		{
			// APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CustomSkeletalMesh->GetOwner());
			// UBaseGameplayAbility* BaseAbility = PlayerCharacter->GetBaseAbilitySystemComponent();
			// if (BaseAbility)
			// {
			// 	UE_LOG(LogTemp,Warning,TEXT("valid"));
			// }
			// else
			// {
			// 	UE_LOG(LogTemp,Warning,TEXT("not valid"));
			// }
			// 몽타주 재생 시도, PlayMontage()는 재생된 몽타주의 재생 시간(0보다 큰 값)을 반환함
			 float MontagePlayReturn1 = AnimInstance->Montage_Play(MontageToPlay,Rate);
			 float MontagePlayReturn = ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection, StartTimeSeconds);
			 if (MontagePlayReturn > 0.f)
			 {
			 	// Ability가 취소되는 등의 상황에서 Task delegate를 방송할 수 없는 경우 빠르게 종료
			 	if (!ShouldBroadcastAbilityTaskDelegates())
			 	{
			 		return;
			 	}
			
			 	// Ability가 취소되었을 때 호출될 delegate 바인딩
			 	InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UAbilityTask_PlayMonatageCustom::OnGameplayAbilityCancelled);
			
			 	// 몽타주 블렌딩 아웃(delegate) 바인딩
			 	BlendingOutDelegate.BindUObject(this, &UAbilityTask_PlayMonatageCustom::OnMontageBlendingOut);
			 	AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);
			
			 	// 몽타주 종료(delegate) 바인딩
			 	MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMonatageCustom::OnMontageEnded);
			 	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
			
			 	// 해당 캐릭터의 경우, 로컬 권한에 따라 루트 모션 스케일 적용
			 	APlayerCharacter* Character = Cast<APlayerCharacter>(GetAvatarActor());
			 	if (Character && (Character->GetLocalRole() == ROLE_Authority ||
			 		(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			 	{
			 		Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
			 	}
			
			 	bPlayedMontage = true;
			}
		}
		else
		{
			//ABILITY_LOG(Warning, TEXT("UMyAbilityTask_PlayMontageAndWait: PlayMontage failed! AnimInstance not found."));
		}
	}
	 else
	 {
	 	//ABILITY_LOG(Warning, TEXT("UMyAbilityTask_PlayMontageAndWait: Invalid AbilitySystemComponent."));
	 }
	
	 // 몽타주가 재생되지 않았다면, Ability가 실행되지 않은 것으로 처리하고 OnCancelled delegate를 방송
	 if (!bPlayedMontage)
	 {
	 	//ABILITY_LOG(Warning, TEXT("UMyAbilityTask_PlayMontageAndWait: Ability %s failed to play montage %s; Task Instance Name %s."), 
	 		//*Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
	 	if (ShouldBroadcastAbilityTaskDelegates())
	 	{
	 		OnCancelled.Broadcast();
	 	}
	 }

	 // Ability Task가 Avatar(실제 캐릭터)에서 입력을 대기하도록 설정
	SetWaitingOnAvatar();
	
}
