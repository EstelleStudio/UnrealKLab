// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPrimaryAssetManagerComponent.h"

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

	// TODO: Asset Manager for game features
	
	// TODO: Bind this to a delegate
	OnPrimaryDataLoadComplete();
}

void UKLabPrimaryAssetManagerComponent::OnPrimaryDataLoadComplete()
{
	check(LoadState == EPrimaryDataLoadState::Loading);
	
	UE_LOG(LogLab, Log, TEXT("[%lld] Primary data of GameState load complete, Current primary data is %ws "), GFrameCounter, *CurrentPrimaryData->GetPrimaryAssetId().ToString());

	// TODO: GameFeature

	PostPrimaryDataLoad();
}

void UKLabPrimaryAssetManagerComponent::PostPrimaryDataLoad()
{
	check(LoadState != EPrimaryDataLoadState::Loaded)
	// TODO: GameFeature

	LoadState = EPrimaryDataLoadState::Loaded;

	/* Call delegate functions*/
	PostPrimaryDataLoaded_High.Broadcast(CurrentPrimaryData);
	PostPrimaryDataLoaded_High.Clear();

	PostPrimaryDataLoaded.Broadcast(CurrentPrimaryData);
	PostPrimaryDataLoaded.Clear();

	PostPrimaryDataLoaded_Low.Broadcast(CurrentPrimaryData);
	PostPrimaryDataLoaded_Low.Clear();
}


