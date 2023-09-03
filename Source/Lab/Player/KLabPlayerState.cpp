// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPlayerState.h"

#include "OnlineSubsystemTypes.h"
#include "Common/KLab.h"
#include "GameModes/KLabGameMode.h"
#include "GameModes/KLabPrimaryAssetManagerComponent.h"
#include "GameModes/KLabGameState.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


AKLabPlayerState::AKLabPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKLabPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKLabPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PawnData);
}

void AKLabPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// TODO: Init Ability Subsystem
	
	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AKLabGameState* GameState = Cast<AKLabGameState>(World->GetGameState());
		check (IsValid(GameState))
		UKLabPrimaryAssetManagerComponent* PrimaryAssetComp = GameState->GetPrimaryAssetComp();
		check (IsValid(PrimaryAssetComp))
		PrimaryAssetComp->CallOrRegister_PostPrimaryDataLoaded(FKLabPostPrimaryDataLoaded::FDelegate::CreateUObject(this, &ThisClass::PostPrimaryDataLoaded));
	}
}

void AKLabPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKLabPlayerState::SetPawnData(const UKLabPawnPrimaryData* InPawnData)
{
	check(InPawnData)
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	// TODO : Ability

	ForceNetUpdate();
}

void AKLabPlayerState::PostPrimaryDataLoaded(const UKLabPrimaryDataAsset* PrimaryData)
{
	if (AKLabGameMode* LyraGameMode = GetWorld()->GetAuthGameMode<AKLabGameMode>())
	{
		if (const UKLabPawnPrimaryData* NewPawnData = LyraGameMode->GetPawnDataFromController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogLab, Error, TEXT("Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}

