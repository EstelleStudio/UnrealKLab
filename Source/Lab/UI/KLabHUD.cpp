// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabHUD.h"

AKLabHUD::AKLabHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AKLabHUD::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKLabHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

