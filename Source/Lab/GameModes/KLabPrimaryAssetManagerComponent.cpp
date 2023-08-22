// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPrimaryAssetManagerComponent.h"

UKLabPrimaryAssetManagerComponent::UKLabPrimaryAssetManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UKLabPrimaryAssetManagerComponent::SetCurrentPrimaryAsset(FPrimaryAssetId ExperienceId)
{
	//TODO:
}


// Called when the game starts
void UKLabPrimaryAssetManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKLabPrimaryAssetManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

