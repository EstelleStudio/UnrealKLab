// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KLabActivatableWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "Components/Widget.h"
#include "KLabActivatableWidgetContainerBase.generated.h"

/**
 *  refer to common ui.
 */
UCLASS(Abstract)
class LAB_API UKLabActivatableWidgetContainerBase : public UWidget
{
	GENERATED_BODY()

public:
	UKLabActivatableWidgetContainerBase(const FObjectInitializer& Initializer);

	void RemoveWidget(UKLabActivatableWidget& WidgetToRemove);

	UFUNCTION(BlueprintCallable)
	UKLabActivatableWidget* GetActiveWidget() const;

	const TArray<UKLabActivatableWidget*>& GetWidgetList() const { return WidgetList; }

	int32 GetNumWidgets() const;

	UFUNCTION(BlueprintCallable)
	void ClearWidgets();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UKLabActivatableWidget>> WidgetList;

	UPROPERTY(Transient)
	TObjectPtr<UKLabActivatableWidget> DisplayedWidget;

	UPROPERTY(Transient)
	FUserWidgetPool GeneratedWidgetsPool;

	TSharedPtr<SOverlay> MyOverlay;
	TSharedPtr<SSpacer> MyInputGuard;
	TSharedPtr<SWidgetSwitcher> MySwitcher;
	
	/*
	 * Add Widget Function
	 */
public:
	
	template <typename ActivatableWidgetT = UKLabActivatableWidget>
	ActivatableWidgetT* AddWidget(TSubclassOf<UKLabActivatableWidget> ActivatableWidgetClass)
	{
		// Don't actually add the widget if the cast will fail
		if (ActivatableWidgetClass && ActivatableWidgetClass->IsChildOf<ActivatableWidgetT>())
		{
			return Cast<ActivatableWidgetT>(AddWidgetInternal(ActivatableWidgetClass, [](UKLabActivatableWidget&) {}));
		}
		return nullptr;
	}
	
	template <typename ActivatableWidgetT = UKLabActivatableWidget>
	ActivatableWidgetT* AddWidget(TSubclassOf<UKLabActivatableWidget> ActivatableWidgetClass, TFunctionRef<void(ActivatableWidgetT&)> InstanceInitFunc)
	{
		// Don't actually add the widget if the cast will fail
		if (ActivatableWidgetClass && ActivatableWidgetClass->IsChildOf<ActivatableWidgetT>())
		{
			return Cast<ActivatableWidgetT>(AddWidgetInternal(ActivatableWidgetClass, [&InstanceInitFunc](UKLabActivatableWidget& WidgetInstance)
			{
				InstanceInitFunc(*CastChecked<ActivatableWidgetT>(&WidgetInstance));
			}));
		}
		return nullptr;
	}
};

/**
 * 
 */
UCLASS()
class LAB_API UKLabActivatableWidgetContainerStack : public UKLabActivatableWidgetContainerBase
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS()
class LAB_API UKLabActivatableWidgetContainerQueue : public UKLabActivatableWidgetContainerBase
{
	GENERATED_BODY()
};
