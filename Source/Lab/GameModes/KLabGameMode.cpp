// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabGameMode.h"

#include "Common/KLab.h"


// Sets default values
AKLabGameMode::AKLabGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AKLabGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// TODO:
	
	UE_LOG(LogLab, Log, TEXT("[Frame: %lld]\t KLab GameMode Init Game, Map: %ws."), GFrameCounter, *MapName);
}

// Called when the game starts or when spawned
void AKLabGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKLabGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

