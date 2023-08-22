// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabGameState.h"

#include "KLabPrimaryAssetManagerComponent.h"


// Sets default values
AKLabGameState::AKLabGameState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PrimaryAssetManager = CreateDefaultSubobject<UKLabPrimaryAssetManagerComponent>(TEXT("PrimaryAssetManagerComponent"));
}

// Called when the game starts or when spawned
void AKLabGameState::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKLabGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

