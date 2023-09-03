// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "KLabPlayerState.generated.h"

class UKLabPrimaryDataAsset;
class UKLabPawnPrimaryData;

UCLASS()
class LAB_API AKLabPlayerState : public AModularPlayerState
{
	GENERATED_BODY()

public:
	AKLabPlayerState();

	/* override function */
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	/**/
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UKLabPawnPrimaryData* InPawnData);
	
protected:
	virtual void BeginPlay() override;

	/**/
	UPROPERTY(Replicated)
	TObjectPtr<const UKLabPawnPrimaryData> PawnData = nullptr;

private:
	void PostPrimaryDataLoaded(const UKLabPrimaryDataAsset* PrimaryData);

};
