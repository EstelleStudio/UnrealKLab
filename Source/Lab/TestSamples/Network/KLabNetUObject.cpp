// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabNetUObject.h"

#include "KLabNetPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

void UKLabNetUObject::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, RandomValueOnServer);

	DOREPLIFETIME(ThisClass, ArrayOf8Bytes);
	DOREPLIFETIME(ThisClass, StructOf8Bytes);
	DOREPLIFETIME(ThisClass, Byte0);
	DOREPLIFETIME(ThisClass, Byte1);
	DOREPLIFETIME(ThisClass, Byte2);
	DOREPLIFETIME(ThisClass, Byte3);
	DOREPLIFETIME(ThisClass, Byte4);
	DOREPLIFETIME(ThisClass, Byte5);
	DOREPLIFETIME(ThisClass, Byte6);
	DOREPLIFETIME(ThisClass, Byte7);
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
	return true;
}

AActor* UKLabNetUObject::GetOwnerActor() const
{
	return GetTypedOuter<AActor>();
}

void UKLabNetUObject::OnRep_RandomValueOnServer()
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%lld] Get server random value RepNotify."), GFrameCounter));
}

void UKLabNetUObject::UpdateReplicatedBytes(int Index)
{
	Index = FMath::Clamp(Index, 0, 7);
	
	auto UpdateLambda = [Index](uint8* Ptr)
	{
		Ptr[Index] = !Ptr[Index];
	};
	
	UpdateLambda(&ArrayOf8Bytes[0]);
	UpdateLambda(&StructOf8Bytes.Byte0);
	UpdateLambda(&Byte0);
}

void UKLabNetUObject::ServerMulticast_Implementation(uint64 ServerFrameCounter)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%lld] Get server multicast RPC, server frame: %lld"), GFrameCounter, ServerFrameCounter));
}

void UKLabNetUObject::ClientToServer_Implementation(uint64 ClientFrameCounter)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%lld] Get client to server RPC, client frame: %lld"), GFrameCounter, ClientFrameCounter));
}
