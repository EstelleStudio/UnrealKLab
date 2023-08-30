// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KLabNetActor.generated.h"

class UKLabNetUObject;

UCLASS()
class LAB_API AKLabNetActor : public AActor
{
	GENERATED_BODY()

public:
	AKLabNetActor();
	friend class UKLabNetUObject;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	void CreateNetSubObject();
	void CleanNetSubObject();

	void StartNetTest();
	void EndNetTest();
	void UpdateServer();
	void ServerRPC();
	void ClientRPC();

	TObjectPtr<UKLabNetUObject> NetObj;
	
	FTimerHandle ServerUpdateTimer;
	FTimerHandle ServerRPCTimer;
	FTimerHandle ClientRPCTimer;
	float RandomValueInterval = 2.f;
	float ServerMulticastInterval = 3.f;
	float ClientToServerInterval = 4.f;
};
