// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabGameMode.h"

#include "Common/KLab.h"
#include "GameFramework/GameSession.h"
#include "System/KLabDebugSystem.h"


// Sets default values
AKLabGameMode::AKLabGameMode()
{
	KLAB_DEBUG_REGISTER();
}

AKLabGameMode::~AKLabGameMode()
{
	//TODO:
}

void AKLabGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::InitExperience);
	
	KLAB_DEBUG_ADDSTR(FString::Printf(TEXT(
		"[Frame: %lld] KLabGameMode InitGame().\n"
		"Firstly, GameModeBase spawn GameSession in current world, Then KLabGameMode register a next-tick-called function to initialize PrimaryAssets of this map (Experience).\n"
		"Map: %ws\n"
		"Options: %ws\n"
		"GameSession Name: %ws\n"),
		GFrameCounter, *MapName, *Options, *GameSession->SessionName.ToString()));
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

void AKLabGameMode::InitExperience()
{
	//TODO:
}

