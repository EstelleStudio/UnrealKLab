// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "KLabEditorSettings.generated.h"

UCLASS(Config=KLabEditor, DefaultConfig, meta=(DisplayName="KLabEditor"), MinimalAPI)
class UKLabEditorSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
	UKLabEditorSettings();
public:
	// The primary assets override to use for Play in Editor (if not set, the default for the world settings of the open map will be used)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, config, Category = KLabEditor, meta=(AllowedClasses="KLabPrimaryDataAsset"))
	FPrimaryAssetId EditorPrimaryAsset;
	
};
