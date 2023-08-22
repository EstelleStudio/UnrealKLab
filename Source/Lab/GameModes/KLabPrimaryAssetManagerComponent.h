// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "KLabPrimaryAssetManagerComponent.generated.h"


UCLASS()
class UKLabPrimaryAssetManagerComponent final : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UKLabPrimaryAssetManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetCurrentPrimaryAsset(FPrimaryAssetId ExperienceId);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
