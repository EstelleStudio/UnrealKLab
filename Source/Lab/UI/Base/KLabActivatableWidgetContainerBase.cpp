// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabActivatableWidgetContainerBase.h"

#include "Common/KLab.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

namespace KLabActivatableWidgetContainerHelper
{
	UKLabActivatableWidget* ActivatableWidgetFromSlate(const TSharedPtr<SWidget>& SlateWidget)
	{
		if (SlateWidget && SlateWidget != SNullWidget::NullWidget && ensure(SlateWidget->GetType().IsEqual(TEXT("SObjectWidget"))))
		{
			UKLabActivatableWidget* ActivatableWidget = Cast<UKLabActivatableWidget>(StaticCastSharedPtr<SObjectWidget>(SlateWidget)->GetWidgetObject());
			if (ensure(ActivatableWidget))
			{
				return ActivatableWidget;
			}
		}
		return nullptr;
	}	
}

UKLabActivatableWidgetContainerBase::UKLabActivatableWidgetContainerBase(const FObjectInitializer& Initializer) : Super(Initializer)
{
	SetVisibilityInternal(ESlateVisibility::Collapsed);
}

void UKLabActivatableWidgetContainerBase::RemoveWidget(UKLabActivatableWidget& WidgetToRemove)
{
	if (&WidgetToRemove == GetActiveWidget())
	{
		// To remove the active widget, just deactivate it (if it's already deactivated, then we're already in the process of ditching it)
		if (WidgetToRemove.IsActivated())
		{
			WidgetToRemove.DeactivateWidget();
		}
		else
		{
			bRemoveDisplayedWidgetPostTransition = true;
		}
	}
	else
	{
		// Otherwise if the widget isn't actually being shown right now, yank it right on out
		TSharedPtr<SWidget> CachedWidget = WidgetToRemove.GetCachedWidget();
		if (CachedWidget && MySwitcher)
		{
			ReleaseWidget(CachedWidget.ToSharedRef());
		}
	}
}

UKLabActivatableWidget* UKLabActivatableWidgetContainerBase::GetActiveWidget() const
{
	return MySwitcher ? KLabActivatableWidgetContainerHelper::ActivatableWidgetFromSlate(MySwitcher->GetActiveWidget()) : nullptr;
}

int32 UKLabActivatableWidgetContainerBase::GetNumWidgets() const
{
	return WidgetList.Num();
}

void UKLabActivatableWidgetContainerBase::ClearWidgets()
{
	SetSwitcherIndex(0);
}

TSharedRef<SWidget> UKLabActivatableWidgetContainerBase::RebuildWidget()
{
	MyOverlay = SNew(SOverlay)
	+ SOverlay::Slot()
	[
		SAssignNew(MySwitcher, SWidgetSwitcher)
	];
	// this intend to intercept all input when switcher is transitioning
	//
	// + SOverlay::Slot()
	// [
	// 	SAssignNew(MyInputGuard, SSpacer)
	// 	.Visibility(EVisibility::Collapsed)
	// ];

	// We always want a 0th slot to be able to animate the first real entry in and out
	MySwitcher->AddSlot()[SNullWidget::NullWidget];
	
	return MyOverlay.ToSharedRef();
}

void UKLabActivatableWidgetContainerBase::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyOverlay.Reset();
	MySwitcher.Reset();
	ReleasedWidgets.Empty();
	GeneratedWidgetsPool.ReleaseAllSlateResources();
}

void UKLabActivatableWidgetContainerBase::SetSwitcherIndex(int32 TargetIndex)
{
	if (MySwitcher && MySwitcher->GetActiveWidgetIndex() != TargetIndex)
	{
		if (DisplayedWidget)
		{
			DisplayedWidget->OnDeactivated().RemoveAll(this);
			if (DisplayedWidget->IsActivated())
			{
				DisplayedWidget->DeactivateWidget();
			}
			else if (MySwitcher->GetActiveWidgetIndex() != 0)
			{
				// The displayed widget has already been deactivated by something other than us, so it should be removed from the container
				// We still need it to remain briefly though until we transition to the new index - then we can remove this entry's slot
				bRemoveDisplayedWidgetPostTransition = true;
			}
		}

		MySwitcher->SetActiveWidgetIndex(TargetIndex);
		// TODO: Here is different from common ui, Find a way to bind this handle to a delegate like OnActiveIndexChanged in MySwitcher?
		HandleActiveIndexChanged(TargetIndex);
	}
}

UKLabActivatableWidget* UKLabActivatableWidgetContainerBase::AddWidgetInternal(TSubclassOf<UKLabActivatableWidget> ActivatableWidgetClass, TFunctionRef<void(UKLabActivatableWidget&)> InitFunc)
{
	if (UKLabActivatableWidget* WidgetInstance = GeneratedWidgetsPool.GetOrCreateInstance(ActivatableWidgetClass))
	{
		InitFunc(*WidgetInstance);
		RegisterInstanceInternal(*WidgetInstance);
		return WidgetInstance;
	}
	return nullptr;
}

void UKLabActivatableWidgetContainerBase::RegisterInstanceInternal(UKLabActivatableWidget& InWidget)
{
	if (ensure(!WidgetList.Contains(&InWidget)))
	{
		WidgetList.Add(&InWidget);
		OnWidgetAddedToList(InWidget);
	}
}

void UKLabActivatableWidgetContainerBase::HandleActiveIndexChanged(int32 ActiveWidgetIndex)
{
	// Remove all slots above the currently active one and release the widgets back to the pool
	while (MySwitcher->GetNumWidgets() - 1 > ActiveWidgetIndex)
	{
		TSharedPtr<SWidget> WidgetToRelease = MySwitcher->GetWidget(MySwitcher->GetNumWidgets() - 1);
		if (ensure(WidgetToRelease))
		{
			ReleaseWidget(WidgetToRelease.ToSharedRef());
		}
	}

	// Also remove the widget that we just transitioned away from if desired
	if (DisplayedWidget && bRemoveDisplayedWidgetPostTransition)
	{
		if (TSharedPtr<SWidget> DisplayedSlateWidget = DisplayedWidget->GetCachedWidget())
		{
			ReleaseWidget(DisplayedSlateWidget.ToSharedRef());
		}
	}
	bRemoveDisplayedWidgetPostTransition = false;

	// Activate the widget that's now being displayed
	DisplayedWidget = KLabActivatableWidgetContainerHelper::ActivatableWidgetFromSlate(MySwitcher->GetActiveWidget());
	if (DisplayedWidget)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		DisplayedWidget->OnDeactivated().AddUObject(this, &ThisClass::HandleActiveWidgetDeactivated, ToRawPtr(DisplayedWidget));
		DisplayedWidget->ActivateWidget();

		if (UWorld* MyWorld = GetWorld())
		{
			FTimerManager& TimerManager = MyWorld->GetTimerManager();
			TimerManager.SetTimerForNextTick(FSimpleDelegate::CreateWeakLambda(this, [this]() { InvalidateLayoutAndVolatility(); }));
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UKLabActivatableWidgetContainerBase::HandleActiveWidgetDeactivated(UKLabActivatableWidget* DeactivatedWidget)
{
	// When the currently displayed widget deactivates, transition the switcher to the preceding slot (if it exists)
	// We'll clean up this slot once the switcher index actually changes
	if (ensure(DeactivatedWidget == DisplayedWidget) && MySwitcher && MySwitcher->GetActiveWidgetIndex() > 0)
	{
		DisplayedWidget->OnDeactivated().RemoveAll(this);
		MySwitcher->SetActiveWidgetIndex(MySwitcher->GetActiveWidgetIndex() - 1);
	}
}

void UKLabActivatableWidgetContainerBase::ReleaseWidget(const TSharedRef<SWidget>& WidgetToRelease)
{
	if (UKLabActivatableWidget* ActivatableWidget = KLabActivatableWidgetContainerHelper::ActivatableWidgetFromSlate(WidgetToRelease))
	{
		GeneratedWidgetsPool.Release(ActivatableWidget, true);
		WidgetList.Remove(ActivatableWidget);
	}

	if (MySwitcher->RemoveSlot(WidgetToRelease) != INDEX_NONE)
	{
		ReleasedWidgets.Add(WidgetToRelease);
		if (ReleasedWidgets.Num() == 1)
		{
			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this,
				[this](float)
				{
					QUICK_SCOPE_CYCLE_COUNTER(STAT_UCommonActivatableWidgetContainerBase_ReleaseWidget);
					ReleasedWidgets.Reset();
					return false;
				}));
		}
	}
}

void UKLabActivatableWidgetContainerBase::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (!IsDesignTime())
	{
		// When initially created, fake that we just did an initial transition to index 0
		HandleActiveIndexChanged(0);
	}
}

void UKLabActivatableWidgetContainerStack::OnWidgetAddedToList(UKLabActivatableWidget& AddedWidget)
{
	// Here is implementation of add a widget to layer:
	if (MySwitcher)
	{
		MySwitcher->AddSlot()[AddedWidget.TakeWidget()]; // Refer to TPanelChildren in Children.h, Search "operator[](const TSharedRef<SWidget>& InChildWidget)".
		SetSwitcherIndex(MySwitcher->GetNumWidgets() - 1);
	}
}

void UKLabActivatableWidgetContainerQueue::OnWidgetAddedToList(UKLabActivatableWidget& AddedWidget)
{
	UE_LOG(LogLab, Error, TEXT("Called activatable widget container queue which has not been implementated."));
}
