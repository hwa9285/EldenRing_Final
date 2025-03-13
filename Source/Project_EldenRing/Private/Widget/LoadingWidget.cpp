// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LoadingWidget.h"

#include "Components/Slider.h"

void ULoadingWidget::UpdateProgressBar(float Progress)
{
	if (LoadingProgressBar)
	{
		LoadingProgressBar->SetValue(Progress);
	}
}
