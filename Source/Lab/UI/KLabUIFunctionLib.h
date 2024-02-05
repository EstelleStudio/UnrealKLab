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

public:
	UFUNCTION(BlueprintCallable)
	static UKLabActivatableWidget* PushWidgetToLayerForPlayer(UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UKLabActivatableWidget> WidgetClass,
		UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName);
};
