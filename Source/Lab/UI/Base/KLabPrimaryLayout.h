// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "KLabActivatableWidget.h"
#include "KLabActivatableWidgetContainerBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "UI/KLabWidgetBase.h"
#include "KLabPrimaryLayout.generated.h"

class UKLabActivatableWidget;
/**
 * The state of an async load operation for the UI.
 */
enum class EAsyncWidgetLayerState : uint8
{
	Canceled,
	Initialize,
	AfterPush
};

class UKLabActivatableWidgetContainerBase;
/**
 * 
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class LAB_API UKLabPrimaryLayout : public UKLabWidgetBase
{
	GENERATED_BODY()

public:
	UKLabPrimaryLayout(const FObjectInitializer& ObjectInitializer);

protected:
	/** Register a layer that widgets can be pushed onto. */
	UFUNCTION(BlueprintCallable, Category="Layer")
	void RegisterLayer(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UKLabActivatableWidgetContainerBase* LayerWidget);

	// Find the widget if it exists on any of the layers and remove it from the layer.
	void FindAndRemoveWidgetFromLayer(UKLabActivatableWidget* ActivatableWidget);

	// Get the layer widget for the given layer tag.
	UKLabActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerName);
	
private:
	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UKLabActivatableWidgetContainerBase>> Layers;

	/**
	 * Push Widget to Layer with Async or Sync.
	 */
public:
	
	template <typename ActivatableWidgetT = UKLabActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, TSoftClassPtr<UKLabActivatableWidget> ActivatableWidgetClass)
	{
		return PushWidgetToLayerStackAsync<ActivatableWidgetT>(LayerName, ActivatableWidgetClass,
			[](EAsyncWidgetLayerState, ActivatableWidgetT*) {});
	}

	template <typename ActivatableWidgetT = UKLabActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, TSoftClassPtr<UKLabActivatableWidget> ActivatableWidgetClass,
		TFunction<void(EAsyncWidgetLayerState, ActivatableWidgetT*)> StateFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UKLabActivatableWidget>::IsDerived, "Only ActivatableWidgets can be used here");
		
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(ActivatableWidgetClass.ToSoftObjectPath(),
			FStreamableDelegate::CreateWeakLambda(this,[this, LayerName, ActivatableWidgetClass, StateFunc]()
			{
				ActivatableWidgetT* Widget = PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass.Get(), [StateFunc](ActivatableWidgetT& WidgetToInit) {
					StateFunc(EAsyncWidgetLayerState::Initialize, &WidgetToInit);
				});

				StateFunc(EAsyncWidgetLayerState::AfterPush, Widget);
			})
		);

		// Setup a cancel delegate so that we can resume input if this handler is canceled.
		StreamingHandle->BindCancelDelegate(FStreamableDelegate::CreateWeakLambda(this,
			[this, StateFunc]()
			{
				StateFunc(EAsyncWidgetLayerState::Canceled, nullptr);
			})
		);

		return StreamingHandle;
	}

	template <typename ActivatableWidgetT = UKLabActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT&) {});
	}

	template <typename ActivatableWidgetT = UKLabActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass, TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UKLabActivatableWidget>::IsDerived, "Only ActivatableWidgets can be used here");

		if (UKLabActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
		{
			return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);
		}

		return nullptr;
	}
};
