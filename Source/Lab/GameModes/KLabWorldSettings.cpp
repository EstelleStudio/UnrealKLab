// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabWorldSettings.h"


// Sets default values
AKLabWorldSettings::AKLabWorldSettings()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKLabWorldSettings::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKLabWorldSettings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

