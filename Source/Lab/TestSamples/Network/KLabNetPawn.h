// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KLabNetPawn.generated.h"

class UKLabNetUObject;

UCLASS()
class LAB_API AKLabNetPawn : public APawn
{
	GENERATED_BODY()
public:
	AKLabNetPawn();
	friend class UKLabNetUObject;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

   virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Replicated)
	TObjectPtr<UKLabNetUObject> NetObj;
private:
	void CreateNetSubObject();
	void CleanNetSubObject();

	void StartNetTest();
	void EndNetTest();

	void ReplicatedBytesTest();
	
	void UpdateServer();
	void ServerRPC();
	void ClientRPC();

	FTimerHandle ReplicatedBytesTestTimer;
	FTimerHandle ServerUpdateTimer;
	FTimerHandle ServerRPCTimer;
	FTimerHandle ClientRPCTimer;

	float ReplicatedBytesTestInterval = 1.f;
	float RandomValueInterval = 2.f;
	float ServerMulticastInterval = 3.f;
	float ClientToServerInterval = 4.f;
};
