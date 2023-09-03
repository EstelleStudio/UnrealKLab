// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabGameMode.h"

#include "KLabGameState.h"
#include "KLabPrimaryAssetManagerComponent.h"
#include "Character/Components/KLabPawnComponent.h"
#include "Common/KLab.h"
#include "GameFramework/GameSession.h"
#include "Development/KLabEditorDebugSystem.h"
#include "Development/KLabEditorSettings.h"
#include "Engine/AssetManager.h"


// Sets default values
AKLabGameMode::AKLabGameMode()
{
	KLAB_DEBUG_REGISTER();
	GameStateClass = AKLabGameState::StaticClass();
}

AKLabGameMode::~AKLabGameMode()
{
	KLAB_DEBUG_UNREGISTER();
}

void AKLabGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::InitPrimaryAssets);
	
	KLAB_DEBUG_ADDSTR(FString::Printf(TEXT(
		"[Frame: %lld] KLabGameMode InitGame().\n"
		"Firstly, GameModeBase spawn GameSession in current world, Then KLabGameMode register a next-tick-called function to initialize PrimaryAssets of this map (Lyra Experience).\n"
		"Map: %ws\n"
		"Options: %ws\n"
		"GameSession Name: %ws\n"),
		GFrameCounter, *MapName, *Options, *GameSession->SessionName.ToString()));
}

void AKLabGameMode::BeginPlay()
{
	Super::BeginPlay();
}

APawn* AKLabGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
 	SpawnInfo.Instigator = GetInstigator();
 	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	SpawnInfo.bDeferConstruction = true;
	
 	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	if (APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
	{
		if (UKLabPawnComponent* PawnComponent = UKLabPawnComponent::GetFromOwner(ResultPawn))
		{
				
		}
		ResultPawn->FinishSpawning(SpawnTransform);
	}
	
	UE_LOG(LogLab, Error, TEXT("Spawn default pawn failed."));
 	return nullptr;
}

void AKLabGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const UKLabPawnPrimaryData* AKLabGameMode::GetPawnDataFromController(AController* InController)
{
	if (IsValid(InController))
	{
		// TODO:
	}
	return nullptr;
}

void AKLabGameMode::InitPrimaryAssets()
{
	FPrimaryAssetId KLabPrimaryAssetId;
	FString KLabPrimaryAssetSource;
	
	GetPrimaryAssetID(KLabPrimaryAssetId, KLabPrimaryAssetSource);
	SetPrimaryAssetsToGameState(KLabPrimaryAssetId);
	
	KLAB_DEBUG_ADDSTR(FString::Printf(TEXT(
		"[Frame: %lld] KLabGameMode InitPrimaryAssets().\n"
		"KLab Primary Assets Id Name: %ws"
		"KLab Primary Assets Id Type Name: %ws"
		"KLab Primary Assets Source Name: %ws"),
		GFrameCounter, *KLabPrimaryAssetId.PrimaryAssetName.ToString(), *KLabPrimaryAssetId.PrimaryAssetType.GetName().ToString(), *KLabPrimaryAssetSource));
}

void AKLabGameMode::GetPrimaryAssetID(FPrimaryAssetId& OutId, FString& OutSourceName)
{
	//Precedence Order
	// - Editor (Developer Settings)
	// - World Settings
	// - Default

	UWorld* World = GetWorld();

	if (!OutId.IsValid() && World->IsPlayInEditor())
	{
		OutId = GetDefault<UKLabEditorSettings>()->EditorPrimaryAsset;
		OutSourceName = TEXT("DeveloperSettings");
	}

	// TODO:
	// see if the world settings has a default experience
	if (!OutId.IsValid())
	{
	}

	// Final fallback to the default experience
	if (!OutId.IsValid())
	{
		//ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LyraExperienceDefinition"), FName("B_LyraDefaultExperience"));
		OutSourceName = TEXT("Default");
	}
	
}

void AKLabGameMode::SetPrimaryAssetsToGameState(FPrimaryAssetId& KLabPrimaryAssetId)
{
	if (KLabPrimaryAssetId.IsValid())
	{
		UKLabPrimaryAssetManagerComponent* PrimaryAssetManagerComp = GameState->FindComponentByClass<UKLabPrimaryAssetManagerComponent>();
		check(PrimaryAssetManagerComp);
		PrimaryAssetManagerComp->SetCurrentPrimaryAsset(KLabPrimaryAssetId);
	}
	else
	{
		UE_LOG(LogLab, Error, TEXT("Lab Primary Data Asset is not valid."));
	}
}

