// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

class ULoadingWidget;
class UStatManager;
/**
 * 
 */
UCLASS()
class PROJECT_ELDENRING_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

	void OnLoadLevelCompleted(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);

	void UpdateLoadingProgress();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	// 로딩 UI 인스턴스
	UPROPERTY()
	UUserWidget* LoadingWidget;

	// 실제 Progress Bar 업데이트를 위한 커스텀 위젯(캐스팅해서 사용)
	UPROPERTY()
	ULoadingWidget* CustomLoadingWidget;

	// 진행률 값 (0.0 ~ 1.0)
	float CurrentProgress;

	// 진행률 업데이트 타이머 핸들
	FTimerHandle ProgressUpdateTimerHandle;
	
public:
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category = "SetPlayerMesh")
	USkeletalMeshComponent* SetPlayerMesh();

	UFUNCTION(BlueprintCallable, Category = "LoadingFunction")
	void LoadNewLevel(FName LevelName);
};
