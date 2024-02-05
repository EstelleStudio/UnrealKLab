// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPrimaryAssetManagerComponent.h"

#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "KLabPrimaryDataAsset.h"
#include "Common/KLab.h"
#include "System/KLabAssetManager.h"

UKLabPrimaryAssetManagerComponent::UKLabPrimaryAssetManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UKLabPrimaryAssetManagerComponent::SetCurrentPrimaryAsset(FPrimaryAssetId Id)
{
	UKLabAssetManager& AssetManager = UKLabAssetManager::Get();
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(Id);
	TSubclassOf<UKLabPrimaryDataAsset> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	
	check(AssetClass);
	const UKLabPrimaryDataAsset* PrimaryDataAsset = GetDefault<UKLabPrimaryDataAsset>(AssetClass);

	CurrentPrimaryData = PrimaryDataAsset;
	StartPrimaryDataLoad();
}

const UKLabPrimaryDataAsset* UKLabPrimaryAssetManagerComponent::GetPrimaryDataAsset() const
{
	check(LoadState == EPrimaryDataLoadState::Loaded);
	check(CurrentPrimaryData != nullptr);
	return CurrentPrimaryData;
}

void UKLabPrimaryAssetManagerComponent::CallOrRegister_PostPrimaryDataLoaded_HighPriority(
	FKLabPostPrimaryDataLoaded::FDelegate&& Delegate)
{
	if (IsPrimaryDataLoaded())
	{
		Delegate.Execute(CurrentPrimaryData);
	}
	else
	{
		PostPrimaryDataLoaded_High.Add(MoveTemp(Delegate));	
	}
}

void UKLabPrimaryAssetManagerComponent::CallOrRegister_PostPrimaryDataLoaded(
	FKLabPostPrimaryDataLoaded::FDelegate&& Delegate)
{
	if (IsPrimaryDataLoaded())
 	{
 		Delegate.Execute(CurrentPrimaryData);
 	}
 	else
 	{
 		PostPrimaryDataLoaded.Add(MoveTemp(Delegate));	
 	}
}

void UKLabPrimaryAssetManagerComponent::CallOrRegister_PostPrimaryDataLoaded_LowPriority(
	FKLabPostPrimaryDataLoaded::FDelegate&& Delegate)
{
	if (IsPrimaryDataLoaded())
    {
    	Delegate.Execute(CurrentPrimaryData);
    }
    else
    {
    	PostPrimaryDataLoaded_Low.Add(MoveTemp(Delegate));	
    }
}

bool UKLabPrimaryAssetManagerComponent::IsPrimaryDataLoaded()
{
	return LoadState == EPrimaryDataLoadState::Loaded && CurrentPrimaryData != nullptr;
}

void UKLabPrimaryAssetManagerComponent::StartPrimaryDataLoad()
{
	check(CurrentPrimaryData != nullptr);
	check(LoadState == EPrimaryDataLoadState::Unloaded);

	UE_LOG(LogLab, Log, TEXT("[%lld] Primary data of GameState start load, Current primary data is %ws "), GFrameCounter, *CurrentPrimaryData->GetPrimaryAssetId().ToString());

	LoadState = EPrimaryDataLoadState::Loading;
	// TODO: Additional action sets
	
	// TODO: Bind this to a delegate
	OnPrimaryDataLoadComplete();
}

void UKLabPrimaryAssetManagerComponent::OnPrimaryDataLoadComplete()
{
	check(LoadState == EPrimaryDataLoadState::Loading);

	// Find the URLs for our GameFeaturePlugins - filtering out dupes and ones that don't have a valid mapping

	for (const FString& PluginName : CurrentPrimaryData->GameFeatures)
	{
		FString PluginURL;
		if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, /*out*/ PluginURL))
		{
			GameFeaturePluginURLs.AddUnique(PluginURL);
		}
		else
		{
			ensureMsgf(false, TEXT("OnPrimaryDataLoadComplete failed to find plugin URL from PluginName %s for primary data asset %s - fix data, ignoring for this run"),
				*PluginName, *CurrentPrimaryData->GetPrimaryAssetId().ToString());
		}
	}

	// Load and activate the features
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading > 0)
	{
		LoadState = EPrimaryDataLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		PostPrimaryDataLoad();
	}
}

void UKLabPrimaryAssetManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	// decrement the number of plugins that are loading
	NumGameFeaturePluginsLoading--;

	if (NumGameFeaturePluginsLoading == 0)
	{
		PostPrimaryDataLoad();
	}
}

void UKLabPrimaryAssetManagerComponent::PostPrimaryDataLoad()
{
	check(LoadState != EPrimaryDataLoadState::Loaded)
	
	UE_LOG(LogLab, Log, TEXT("[%lld] Primary data of GameState load complete, Current primary data is %ws "), GFrameCounter, *CurrentPrimaryData->GetPrimaryAssetId().ToString());
	
	// Execute the actions
	LoadState = EPrimaryDataLoadState::ExecutingActions;
	
	FGameFeatureActivatingContext Context;
	// Only apply to our specific world context if set
	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	if (ExistingWorldContext)
	{
		Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	}

	for (UGameFeatureAction* Action : CurrentPrimaryData->Actions)
	{
		if (Action != nullptr)
		{
			// From lyra:
			// The fact that these don't take a world are potentially problematic in client-server PIE,
			// The current behavior matches systems like gameplay tags where loading and registering apply to the entire process,
			// but actually applying the results to actors is restricted to a specific world
			Action->OnGameFeatureRegistering();
			Action->OnGameFeatureLoading();
			Action->OnGameFeatureActivating(Context);
		}
	}

	// TODO:  Additional action sets
	
	// Full Load Completed
	LoadState = EPrimaryDataLoadState::Loaded;

	/* Call delegate functions*/
	PostPrimaryDataLoaded_High.Broadcast(CurrentPrimaryData);
	PostPrimaryDataLoaded_High.Clear();

	PostPrimaryDataLoaded.Broadcast(CurrentPrimaryData);
	PostPrimaryDataLoaded.Clear();

	PostPrimaryDataLoaded_Low.Broadcast(CurrentPrimaryData);
	PostPrimaryDataLoaded_Low.Clear();

	// TODO: Local Settings ?
}


