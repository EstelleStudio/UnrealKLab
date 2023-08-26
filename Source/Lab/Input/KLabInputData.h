// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "KLabInputData.generated.h"

class UInputAction;
/*
 * KLab Input Action, for mapping a input action to a input gameplay tag.
 */
USTRUCT(BlueprintType)
struct FKLabInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction =nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS(BlueprintType, Const, meta=(DisplayName="KLab Input Config"))
class LAB_API UKLabInputData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FKLabInputAction> ArrayOfNativeInputAction;
	
};
