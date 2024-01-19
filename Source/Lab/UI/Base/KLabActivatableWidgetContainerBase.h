// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KLabActivatableWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "Components/Widget.h"
#include "KLabActivatableWidgetContainerBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LAB_API UKLabActivatableWidgetContainerBase : public UWidget
{
	GENERATED_BODY()

public:

	void RemoveWidget(UKLabActivatableWidget& WidgetToRemove);

	UFUNCTION(BlueprintCallable)
	UKLabActivatableWidget* GetActiveWidget() const;

	const TArray<UKLabActivatableWidget*>& GetWidgetList() const { return WidgetList; }

	int32 GetNumWidgets() const;

	UFUNCTION(BlueprintCallable)
	void ClearWidgets();

	void SetTransitionDuration(float Duration);
	float GetTransitionDuration() const;

protected:
	/** The total duration of a single transition between widgets */
	UPROPERTY(EditAnywhere)
	float TransitionDuration = 0.4f;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UKLabActivatableWidget>> WidgetList;

	UPROPERTY(Transient)
	TObjectPtr<UKLabActivatableWidget> DisplayedWidget;

	UPROPERTY(Transient)
	FUserWidgetPool GeneratedWidgetsPool;
	
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
