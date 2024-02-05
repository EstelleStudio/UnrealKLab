// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeaturePluginOperationResult.h"
#include "Components/GameStateComponent.h"
#include "KLabPrimaryAssetManagerComponent.generated.h"

class UKLabPrimaryDataAsset;
DECLARE_MULTICAST_DELEGATE_OneParam(FKLabPostPrimaryDataLoaded, const UKLabPrimaryDataAsset*)

UCLASS()
class UKLabPrimaryAssetManagerComponent final : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UKLabPrimaryAssetManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/**/
	void SetCurrentPrimaryAsset(FPrimaryAssetId Id);
	const UKLabPrimaryDataAsset* GetPrimaryDataAsset() const;

	/* On primary data asset loaded finished */
	void CallOrRegister_PostPrimaryDataLoaded_HighPriority(FKLabPostPrimaryDataLoaded::FDelegate&& Delegate);
	void CallOrRegister_PostPrimaryDataLoaded(FKLabPostPrimaryDataLoaded::FDelegate&& Delegate);
	void CallOrRegister_PostPrimaryDataLoaded_LowPriority(FKLabPostPrimaryDataLoaded::FDelegate&& Delegate);

	/**/
	bool IsPrimaryDataLoaded();
	
private:
	void StartPrimaryDataLoad();
	void OnPrimaryDataLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void PostPrimaryDataLoad();
	
	enum class EPrimaryDataLoadState
	{
		Unloaded,
		Loading,
		LoadingGameFeatures,
		ExecutingActions,
		Loaded,
		Deactivating,
	};

	EPrimaryDataLoadState LoadState = EPrimaryDataLoadState::Unloaded;

	//TODO: Replicated
	UPROPERTY()
	TObjectPtr<const UKLabPrimaryDataAsset> CurrentPrimaryData;

	TArray<FString> GameFeaturePluginURLs; // All game features list
	int32 NumGameFeaturePluginsLoading = 0; // Loading game features num

	/**
	 * Delegate called when the experience has finished loading just before others
	 * (e.g., subsystems that set up for regular gameplay)
	 */
	FKLabPostPrimaryDataLoaded PostPrimaryDataLoaded_High;
	
	/** Delegate called when the experience has finished loading */
	FKLabPostPrimaryDataLoaded PostPrimaryDataLoaded;
	
	/** Delegate called at last*/
	FKLabPostPrimaryDataLoaded PostPrimaryDataLoaded_Low;
	
};
