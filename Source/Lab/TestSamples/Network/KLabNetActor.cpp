// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabNetActor.h"
#include "KLabNetUObject.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AKLabNetActor::AKLabNetActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	bReplicateUsingRegisteredSubObjectList = true;

	// This actor has no root component:
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AKLabNetActor::BeginPlay()
{
	Super::BeginPlay();
	CreateNetSubObject();
	StartNetTest();
}

void AKLabNetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndNetTest();
	CleanNetSubObject();
	Super::EndPlay(EndPlayReason);
}

void AKLabNetActor::CreateNetSubObject()
{
	CleanNetSubObject();

	NetObj= NewObject<UKLabNetUObject>(this);
	AddReplicatedSubObject(NetObj);
}

void AKLabNetActor::CleanNetSubObject()
{
	if (IsValid(NetObj))
	{
		RemoveReplicatedSubObject(NetObj);
	}
}

void AKLabNetActor::StartNetTest()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(ServerUpdateTimer, this, &AKLabNetActor::UpdateServer, RandomValueInterval, true, RandomValueInterval);
		// GetWorldTimerManager().SetTimer(ServerRPCTimer, this, &AKLabNetActor::ServerRPC, ServerMulticastInterval, true, ServerMulticastInterval);
	}

	if (GetLocalRole() < ROLE_Authority)
	{
		// GetWorldTimerManager().SetTimer(ClientRPCTimer, this, &AKLabNetActor::ClientRPC, ClientToServerInterval, true, ClientToServerInterval);
	}
}

void AKLabNetActor::EndNetTest()
{
	GetWorldTimerManager().ClearTimer(ServerUpdateTimer);
	GetWorldTimerManager().ClearTimer(ServerRPCTimer);
	GetWorldTimerManager().ClearTimer(ClientRPCTimer);
}

void AKLabNetActor::UpdateServer()
{
	NetObj->RandomValueOnServer = FMath::Rand();
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%lld] Server update random value: %d."), GFrameCounter, NetObj->RandomValueOnServer));
}

void AKLabNetActor::ServerRPC()
{
	NetObj->ServerMulticast(GFrameCounter);
}

void AKLabNetActor::ClientRPC()
{
	NetObj->ClientToServer(GFrameCounter);
}


