// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "KLabAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class UKLabAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UKLabAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
