// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KLabPrimaryDataAsset.generated.h"

class UKLabPawnPrimaryData;
class UGameFeatureAction;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class LAB_API UKLabPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UKLabPrimaryDataAsset();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	
public:
	// Default pawn data for player.
	UPROPERTY(EditDefaultsOnly, Category="KLab|Gameplay")
	TObjectPtr<UKLabPawnPrimaryData> PawnData;

	// Array of GameFeatures that this primary asset want to active.
	UPROPERTY(EditDefaultsOnly, Category = "KLab|Gameplay")
	TArray<FString> GameFeatures;
	
	// Array of actions to perform as this primary asset is loaded/activated/deactivated/unloaded.
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "KLab|Gameplay")
	TArray<TObjectPtr<UGameFeatureAction>> GameFeatureActions;
	
};
