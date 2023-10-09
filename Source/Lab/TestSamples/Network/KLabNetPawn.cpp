// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabNetPawn.h"
#include "KLabNetUObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AKLabNetPawn::AKLabNetPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	bReplicateUsingRegisteredSubObjectList = true;

	// This actor has no root component:
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AKLabNetPawn::BeginPlay()
{
	Super::BeginPlay();
	CreateNetSubObject();
	StartNetTest();
}

void AKLabNetPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndNetTest();
	CleanNetSubObject();
	Super::EndPlay(EndPlayReason);
}

void AKLabNetPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AKLabNetPawn::CreateNetSubObject()
{
	CleanNetSubObject();
	NetObj= NewObject<UKLabNetUObject>(this);
	AddReplicatedSubObject(NetObj);
}

void AKLabNetPawn::CleanNetSubObject()
{
	if (IsValid(NetObj))
	{
		RemoveReplicatedSubObject(NetObj);
	}
}

void AKLabNetPawn::StartNetTest()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(ReplicatedBytesTestTimer, this, &AKLabNetPawn::ReplicatedBytesTest, ReplicatedBytesTestInterval, true,ReplicatedBytesTestInterval);
		// GetWorldTimerManager().SetTimer(ServerUpdateTimer, this, &AKLabNetPawn::UpdateServer, RandomValueInterval, true, RandomValueInterval);
		// GetWorldTimerManager().SetTimer(ServerRPCTimer, this, &AKLabNetPawn::ServerRPC, ServerMulticastInterval, true, ServerMulticastInterval);
	}

	// if (GetLocalRole() < ROLE_Authority && IsLocallyControlled())
	// {
		// GetWorldTimerManager().SetTimer(ClientRPCTimer, this, &AKLabNetPawn::ClientRPC, ClientToServerInterval, true, ClientToServerInterval);
	// }
}

void AKLabNetPawn::EndNetTest()
{
	GetWorldTimerManager().ClearTimer(ReplicatedBytesTestTimer);
	// GetWorldTimerManager().ClearTimer(ServerUpdateTimer);
	// GetWorldTimerManager().ClearTimer(ServerRPCTimer);
	// GetWorldTimerManager().ClearTimer(ClientRPCTimer);
}

void AKLabNetPawn::ReplicatedBytesTest()
{
	static int Index = 0;
	NetObj->UpdateReplicatedBytes(Index++);
	Index = Index > 7 ? 0 : Index;
}

void AKLabNetPawn::UpdateServer()
{
	NetObj->RandomValueOnServer = FMath::Rand();
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%lld] Server update random value: %d."), GFrameCounter, NetObj->RandomValueOnServer));
}

void AKLabNetPawn::ServerRPC()
{
	NetObj->ServerMulticast(GFrameCounter);
}

void AKLabNetPawn::ClientRPC()
{
	NetObj->ClientToServer(GFrameCounter);
}

void AKLabNetPawn::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NetObj);
}
