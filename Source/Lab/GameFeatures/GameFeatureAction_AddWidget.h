// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "UIExtensionSystem.h"
#include "GameFeatureAction_AddWidget.generated.h"

class UKLabActivatableWidget;
struct FWorldContext;
struct FComponentRequestHandle;

USTRUCT()
struct FKLabHUDLayoutRequest
{
	GENERATED_BODY()

	// The layout widget to spawn
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UKLabActivatableWidget> LayoutClass;

	// The layer to insert the widget in
	UPROPERTY(EditAnywhere, Category=UI, meta=(Categories="UI.Layer"))
	FGameplayTag LayerID;
};


USTRUCT()
struct FKLabHUDElementEntry
{
	GENERATED_BODY()

	// The widget to spawn
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	// The slot ID where we should place this widget
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};

UCLASS(MinimalAPI, meta = (DisplayName = "Add Widgets"))
class UGameFeatureAction_AddWidget : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
	
public:
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif

	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	
private:

	// Layout to add to the HUD
	UPROPERTY(EditAnywhere, Category=UI, meta=(TitleProperty="{LayerID} -> {LayoutClass}"))
	TArray<FKLabHUDLayoutRequest> Layout;

	// Widgets to add to the HUD
	UPROPERTY(EditAnywhere, Category=UI, meta=(TitleProperty="{SlotID} -> {WidgetClass}"))
	TArray<FKLabHUDElementEntry> Widgets;

	struct FPerActorData
	{
		TArray<TWeakObjectPtr<UKLabActivatableWidget>> LayoutsAdded;
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TMap<FObjectKey, FPerActorData> ActorData; 
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

private:
	//
	void Reset(FPerContextData& ActiveData);
	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	//
	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);
};
