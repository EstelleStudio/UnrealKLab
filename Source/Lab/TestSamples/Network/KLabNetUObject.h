// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "KLabNetUObject.generated.h"

USTRUCT()
struct FRepTest8Bytes
{
	GENERATED_BODY()

	UPROPERTY() uint8 Byte0 = 0;
	UPROPERTY() uint8 Byte1 = 0;;
	UPROPERTY() uint8 Byte2 = 0;;
	UPROPERTY() uint8 Byte3 = 0;;
	UPROPERTY() uint8 Byte4 = 0;;
	UPROPERTY() uint8 Byte5 = 0;;
	UPROPERTY() uint8 Byte6 = 0;;
	UPROPERTY() uint8 Byte7 = 0;;
};

/**
 * 
 */
UCLASS()
class LAB_API UKLabNetUObject : public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsSupportedForNetworking() const override;
	virtual AActor* GetOwnerActor() const;

	friend class AKLabNetPawn;

protected:
	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;

protected:
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_RandomValueOnServer , Category = "KLab|Test")
	int RandomValueOnServer = 0;

	UFUNCTION()
	void OnRep_RandomValueOnServer();
	
	UFUNCTION(Server, Reliable)
	void ClientToServer(uint64 ClientFrameCounter);
	
	UFUNCTION(NetMulticast, Reliable)
	void ServerMulticast(uint64 ServerFrameCounter);

	/**
	 * Replicated Bytes Test
	 *
	 * 1. Array
	 * 2. UStruct
	 * 4. 8 Var
	 */

	void UpdateReplicatedBytes(int Index);
	
	UPROPERTY(Replicated)
	TArray<uint8> ArrayOf8Bytes = {0, 0, 0, 0, 0, 0, 0, 0};
	
	UPROPERTY(Replicated)
	FRepTest8Bytes StructOf8Bytes;

	UPROPERTY(Replicated) uint8 Byte0 = 0;;
	UPROPERTY(Replicated) uint8 Byte1 = 0;;
	UPROPERTY(Replicated) uint8 Byte2 = 0;;
	UPROPERTY(Replicated) uint8 Byte3 = 0;;
	UPROPERTY(Replicated) uint8 Byte4 = 0;;
	UPROPERTY(Replicated) uint8 Byte5 = 0;;
	UPROPERTY(Replicated) uint8 Byte6 = 0;;
	UPROPERTY(Replicated) uint8 Byte7 = 0;;
};
