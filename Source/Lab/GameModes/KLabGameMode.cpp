// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabGameMode.h"

#include "KLabGameState.h"
#include "KLabPrimaryAssetManagerComponent.h"
#include "Character/KLabPawn.h"
#include "Character/Components/KLabPawnComponent.h"
#include "Common/KLab.h"
#include "GameFramework/GameSession.h"
#include "Development/KLabEditorSettings.h"
#include "Player/KLabPlayerState.h"
#include "Character/KLabPawnPrimaryData.h"
#include "Kismet/GameplayStatics.h"
#include "Player/KLabPlayerController.h"
#include "UI/KLabHUD.h"

// Sets default values
AKLabGameMode::AKLabGameMode()
{
	GameStateClass = AKLabGameState::StaticClass();
	HUDClass = AKLabHUD::StaticClass();
	PlayerControllerClass = AKLabPlayerController::StaticClass();
	PlayerStateClass = AKLabPlayerState::StaticClass();
	DefaultPawnClass = AKLabPawn::StaticClass();
}

AKLabGameMode::~AKLabGameMode()
{
}

void AKLabGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::InitPrimaryAssets);
}

void AKLabGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AKLabGameMode::InitGameState()
{
	Super::InitGameState();

	AKLabGameState* KLabGameState = Cast<AKLabGameState>(GameState);
	UKLabPrimaryAssetManagerComponent* PrimaryAssetComp = KLabGameState->GetPrimaryAssetComp();
	check(PrimaryAssetComp);

	PrimaryAssetComp->CallOrRegister_PostPrimaryDataLoaded(FKLabPostPrimaryDataLoaded::FDelegate::CreateUObject(this, &ThisClass::PostPrimaryDataLoaded));
}

UClass* AKLabGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// Different from lyra, we only return pawn class of PawnData when InController is a KLab player controller.
	if (InController->StaticClass()->IsChildOf<AKLabPlayerController>())
	{
		if (const UKLabPawnPrimaryData* PawnData = GetPawnDataFromController(InController))
		{
			if (PawnData->PawnClass)
			{
				return PawnData->PawnClass;
			}
		}
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
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
			if (const UKLabPawnPrimaryData* PawnData = GetPawnDataFromController(NewPlayer))
			{
				PawnComponent->SetPawnData(PawnData);
			}
			else
			{
				UE_LOG(LogLab, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(ResultPawn));
			}
		}
		ResultPawn->FinishSpawning(SpawnTransform);

		return ResultPawn;
	}
	
	UE_LOG(LogLab, Error, TEXT("Spawn default pawn failed."));
 	return nullptr;
}

void AKLabGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Delay starting a new player
	// We will start a new player when primary data asset loaded!
	if (IsPrimaryDataAssetLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

void AKLabGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const UKLabPawnPrimaryData* AKLabGameMode::GetPawnDataFromController(AController* InController)
{
	// See if pawn data is already set on the player state
	if (IsValid(InController))
	{
		if (const AKLabPlayerState* PS = InController->GetPlayerState<AKLabPlayerState>())
		{
			if (const UKLabPawnPrimaryData* PawnData = PS->GetPawnData<UKLabPawnPrimaryData>())
			{
				return PawnData;
			}
		}
	}
		
	// If not, fall back to the the default for the current experience
	AKLabGameState* KLabGameState = Cast<AKLabGameState>(GameState);
	UKLabPrimaryAssetManagerComponent* PrimaryAssetComp = KLabGameState->GetPrimaryAssetComp();
	check(PrimaryAssetComp)

	if (PrimaryAssetComp->IsPrimaryDataLoaded())
	{
		const UKLabPrimaryDataAsset* PrimaryData = PrimaryAssetComp->GetPrimaryDataAsset();
		return PrimaryData->PawnData;
	}
	
	return nullptr;
}

void AKLabGameMode::InitPrimaryAssets()
{
	FPrimaryAssetId KLabPrimaryAssetId;
	FString KLabPrimaryAssetSource;
	
	GetPrimaryAssetID(KLabPrimaryAssetId, KLabPrimaryAssetSource);
	SetPrimaryAssetsToGameState(KLabPrimaryAssetId);

	UE_LOG(LogLab, Log, TEXT("[%lld]Init Primary Asset %ws (Source: %ws)"), GFrameCounter, *KLabPrimaryAssetId.ToString(), *KLabPrimaryAssetSource);
}

bool AKLabGameMode::IsPrimaryDataAssetLoaded() const
{
	check(GameState);
	AKLabGameState* KLabGameState = Cast<AKLabGameState>(GameState);
	UKLabPrimaryAssetManagerComponent* PrimaryAssetComp = KLabGameState->GetPrimaryAssetComp();
	check(PrimaryAssetComp);

	return PrimaryAssetComp->IsPrimaryDataLoaded();
}

void AKLabGameMode::PostPrimaryDataLoaded(const UKLabPrimaryDataAsset* PrimaryData)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}	
}

void AKLabGameMode::GetPrimaryAssetID(FPrimaryAssetId& OutId, FString& OutSourceName)
{
	//Precedence Order
	// - URL Options override
	// - Editor (Developer Settings)
	// - World Settings
	// - Default

	if (!OutId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("KLabPrimaryDataAsset")))
	{
		const FString OptionsPrimaryAsset = UGameplayStatics::ParseOption(OptionsString, TEXT("KLabPrimaryDataAsset"));
		OutId = FPrimaryAssetId(FPrimaryAssetType(UKLabPrimaryDataAsset::StaticClass()->GetFName()), FName(*OptionsPrimaryAsset));
		OutSourceName = TEXT("OptionsString");
	}
	
	if (!OutId.IsValid() && GetWorld()->IsPlayInEditor())
	{
		OutId = GetDefault<UKLabEditorSettings>()->EditorPrimaryAsset;
		OutSourceName = TEXT("DeveloperSettings");
	}

	// TODO:
	// see if the world settings has a default experience
	if (!OutId.IsValid())
	{
	}

	// TODO:
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
