// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "KLabNetUObject.generated.h"

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

	friend class AKLabNetActor;
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

};
