// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabActivatableWidgetContainerBase.h"

void UKLabActivatableWidgetContainerBase::RemoveWidget(UKLabActivatableWidget& WidgetToRemove)
{
}

UKLabActivatableWidget* UKLabActivatableWidgetContainerBase::GetActiveWidget() const
{
	return nullptr;
}

int32 UKLabActivatableWidgetContainerBase::GetNumWidgets() const
{
	return 0;
}

void UKLabActivatableWidgetContainerBase::ClearWidgets()
{
}

void UKLabActivatableWidgetContainerBase::SetTransitionDuration(float Duration)
{
}

float UKLabActivatableWidgetContainerBase::GetTransitionDuration() const
{
	return 0.f;
}
