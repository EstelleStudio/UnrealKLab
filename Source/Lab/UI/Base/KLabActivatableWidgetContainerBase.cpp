// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabActivatableWidgetContainerBase.h"

#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

UKLabActivatableWidgetContainerBase::UKLabActivatableWidgetContainerBase(const FObjectInitializer& Initializer) : Super(Initializer)
{
	SetVisibilityInternal(ESlateVisibility::Collapsed);
}

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

TSharedRef<SWidget> UKLabActivatableWidgetContainerBase::RebuildWidget()
{
	MyOverlay = SNew(SOverlay)
	+ SOverlay::Slot()
	[
		SAssignNew(MySwitcher, SWidgetSwitcher)
	]
	+ SOverlay::Slot()
	[
		SAssignNew(MyInputGuard, SSpacer)
		.Visibility(EVisibility::Collapsed)
	];

	// We always want a 0th slot to be able to animate the first real entry in and out
	MySwitcher->AddSlot() [SNullWidget::NullWidget];
	
	return MyOverlay.ToSharedRef();
}

void UKLabActivatableWidgetContainerBase::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyOverlay.Reset();
	MyInputGuard.Reset();
	MySwitcher.Reset();	
}
