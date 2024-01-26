// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KLabUIFunctionLib.generated.h"

struct FGameplayTag;
class UKLabActivatableWidget;
/**
 * 
 */
UCLASS()
class LAB_API UKLabUIFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static bool PushWidgetToLayerForPlayer(UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UKLabActivatableWidget> WidgetClass,
		UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName);
};
