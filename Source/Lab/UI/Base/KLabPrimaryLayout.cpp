// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPrimaryLayout.h"

#include "KLabActivatableWidgetContainerBase.h"

UKLabPrimaryLayout::UKLabPrimaryLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UKLabPrimaryLayout::RegisterLayer(FGameplayTag LayerTag, UKLabActivatableWidgetContainerBase* LayerWidget)
{
	if (!IsDesignTime())
	{
		LayerWidget->SetTransitionDuration(0.0);
		Layers.Add(LayerTag, LayerWidget);
	}
}

void UKLabPrimaryLayout::FindAndRemoveWidgetFromLayer(UKLabActivatableWidget* ActivatableWidget)
{
	for (const auto& Layer : Layers)
	{
		Layer.Value->RemoveWidget(*ActivatableWidget);
	}
}

UKLabActivatableWidgetContainerBase* UKLabPrimaryLayout::GetLayerWidget(FGameplayTag LayerName)
{
	return Layers.FindRef(LayerName);
}
