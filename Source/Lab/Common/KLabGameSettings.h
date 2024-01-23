// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "KLabGameSettings.generated.h"

class UKLabPrimaryLayout;
/**
 * 
 */
UCLASS(config=KLabProj, defaultconfig, MinimalAPI, meta=(DisplayName="KLab Setting"))
class UKLabGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category="KLab|UI")
	TSoftClassPtr<UKLabPrimaryLayout> LayoutClass;
};
