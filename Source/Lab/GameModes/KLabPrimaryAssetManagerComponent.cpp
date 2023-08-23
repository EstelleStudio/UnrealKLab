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

	CurrentPrimaryAsset = PrimaryDataAsset;

	UE_LOG(LogLab, Log, TEXT("%ws"), *CurrentPrimaryAsset->GameFeaturesToEnable[0]);
}


