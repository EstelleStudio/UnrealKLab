// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabAssetManager.h"

#include "Common/KLab.h"

UKLabAssetManager& UKLabAssetManager::Get()
{
	check(GEngine);

	if (UKLabAssetManager* Singleton = Cast<UKLabAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogLab, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to KLabAssetManager!"));

	return *NewObject<UKLabAssetManager>();
}
