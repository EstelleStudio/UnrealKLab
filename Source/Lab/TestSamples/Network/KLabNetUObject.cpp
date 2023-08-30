// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabNetUObject.h"

#include "KLabNetActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

void UKLabNetUObject::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams NetParameters;

	NetParameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, RandomValueOnServer, NetParameters);
}

int32 UKLabNetUObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	if (IsValid(GetOuter()))
	{
		return GetOuter()->GetFunctionCallspace(Function, Stack);
	}
	return Super::GetFunctionCallspace(Function, Stack);
}

bool UKLabNetUObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject))
	if (AActor* Owner = GetOwnerActor())
	{
		if (UNetDriver* NetDriver = Owner->GetNetDriver())
		{
			NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
		}
	}
	
	return false;
}

bool UKLabNetUObject::IsSupportedForNetworking() const
{
	return UObject::IsSupportedForNetworking();
}

AActor* UKLabNetUObject::GetOwnerActor() const
{
	return GetTypedOuter<AActor>();
}

void UKLabNetUObject::OnRep_RandomValueOnServer()
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%lld] Get server random value RepNotify."), GFrameCounter));
}

void UKLabNetUObject::ServerMulticast_Implementation(uint64 ServerFrameCounter)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%lld] Get server multicast RPC, server frame: %lld"), GFrameCounter, ServerFrameCounter));
}


void UKLabNetUObject::ClientToServer_Implementation(uint64 ClientFrameCounter)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%lld] Get client to server RPC, client frame: %lld"), GFrameCounter, ClientFrameCounter));
}
