// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KLabPrimaryDataAsset.generated.h"

class UKLabPawnPrimaryData;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class LAB_API UKLabPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="KLab|Gameplay")
	TObjectPtr<UKLabPawnPrimaryData> PawnData;
	
};
