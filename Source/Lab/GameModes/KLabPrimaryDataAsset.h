// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KLabPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Const)
class LAB_API UKLabPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// List of Game Feature Plugins this experience wants to have active
	UPROPERTY(EditDefaultsOnly, Category = "KLab|Gameplay")
	TArray<FString> GameFeaturesToEnable;

};
