// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "KLabPrimaryAssetManagerComponent.generated.h"


class UKLabPrimaryDataAsset;

UCLASS()
class UKLabPrimaryAssetManagerComponent final : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UKLabPrimaryAssetManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetCurrentPrimaryAsset(FPrimaryAssetId Id);

private:
	UPROPERTY()
	TObjectPtr<const UKLabPrimaryDataAsset> CurrentPrimaryAsset;
};
