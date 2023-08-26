// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "KLabInputComponent.generated.h"

class UPlayerMappableInputConfig;

UCLASS(meta=(BlueprintSpawnableComponent))
class LAB_API UKLabInputComponent : public UPawnComponent
{
	GENERATED_BODY()

	explicit UKLabInputComponent(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(EditAnywhere, Category="KLab|Input")
	TArray<TSoftObjectPtr<UPlayerMappableInputConfig>> ArrayOfInputConfigs;
	
	bool bReadyToBindInputs = false;
};
