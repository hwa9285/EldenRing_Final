// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"

#include "Components/Stat/StatManager.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/LoadingWidget.h"

//test
void UBaseGameInstance::Init()
{
	Super::Init();

}

void UBaseGameInstance::LoadNewLevel(FName LevelName)
{
    // 1. 로딩 UI 생성 및 표시
    if (LoadingWidgetClass)
    {
        LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
        if (LoadingWidget)
        {
            LoadingWidget->AddToViewport();

            // 커스텀 위젯으로 캐스팅 (UMyLoadingWidget은 Progress Bar 업데이트 함수를 갖고 있음)
            CustomLoadingWidget = Cast<ULoadingWidget>(LoadingWidget);
        }
    }

    // 2. 진행률 초기화 및 타이머 시작 (실제 로딩 진행률 대신 시뮬레이션)
    CurrentProgress = 0.0f;
    GetWorld()->GetTimerManager().SetTimer(ProgressUpdateTimerHandle, this, &UBaseGameInstance::UpdateLoadingProgress, 0.1f, true);

    // 3. 로드할 맵의 패키지 경로 구성  
    // 예: "/Game/Maps/LevelName" 형식. 프로젝트에 맞게 수정하세요.
    FString LevelPath = FString::Printf(TEXT("/Game/TestMap/%s"), *LevelName.ToString());

    // 4. 비동기 로딩 시작 (LoadPackageAsync 사용)  
    // PKG_ContainsMap 플래그를 사용하면 맵 로드임을 지정할 수 있음
    LoadPackageAsync(LevelPath, FLoadPackageAsyncDelegate::CreateUObject(this, &UBaseGameInstance::OnLoadLevelCompleted), PKG_ContainsMap);
}

void UBaseGameInstance::UpdateLoadingProgress()
{
    // 실제 진행률 값을 가져오기 어려운 경우, 임의로 진행률을 증가시키는 방법 사용
    // (예: 90%까지 서서히 증가시키고, 완료 콜백에서 100%로 설정)
    if (CurrentProgress < 0.9f)
    {
        CurrentProgress += 0.05f;

        // 위젯이 있으면 Progress Bar 업데이트 함수 호출 (UMyLoadingWidget에 UpdateProgressBar 함수가 있다고 가정)
        if (CustomLoadingWidget)
        {
            CustomLoadingWidget->UpdateProgressBar(CurrentProgress);
        }
    }
}

void UBaseGameInstance::OnLoadLevelCompleted(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
    // 타이머 중지
    GetWorld()->GetTimerManager().ClearTimer(ProgressUpdateTimerHandle);

    // 로딩이 완료되었으므로 진행률을 100%로 설정
    CurrentProgress = 1.0f;
    if (CustomLoadingWidget)
    {
        CustomLoadingWidget->UpdateProgressBar(CurrentProgress);
    }

    // 로딩 UI 제거
    if (LoadingWidget)
    {
        LoadingWidget->RemoveFromParent();
        LoadingWidget = nullptr;
    }

    // 5. 맵 전환  
    // PackageName은 로드된 패키지의 전체 경로 (예: "/Game/Maps/LevelName")이므로, 단축 이름을 추출합니다.
    FString PackageNameString = PackageName.ToString();
    FString LevelShortName = FPackageName::GetShortName(PackageNameString);

    // OpenLevel로 전환 (만약 다른 전환 방식이 필요하면 해당 부분을 변경)
    UGameplayStatics::OpenLevel(this, FName(*LevelShortName));
}