// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KLabPawnPrimaryData.generated.h"

class UKLabInputData;
/**
 * 
 */
UCLASS(BlueprintType, Const, meta=(DisplayName="KLab Pawn Primary Data", ShortToolTip="Data asset to define a KLab pawn."))
class LAB_API UKLabPawnPrimaryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="KLab|Input")
	TObjectPtr<UKLabInputData> InputConfig = nullptr;
};
