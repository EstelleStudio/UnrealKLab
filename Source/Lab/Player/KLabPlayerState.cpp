// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPlayerState.h"


AKLabPlayerState::AKLabPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKLabPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKLabPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKLabPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// TODO: Init Ability Subsystem
	
}

