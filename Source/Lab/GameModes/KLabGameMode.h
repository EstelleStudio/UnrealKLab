// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"

#include "KLabGameMode.generated.h"

class UKLabPawnPrimaryData;
class UKLabPrimaryDataAsset;

UCLASS()
class LAB_API AKLabGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	AKLabGameMode();
	virtual ~AKLabGameMode() override;

	/* override function */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void Tick(float DeltaTime) override;

	/**/
	const UKLabPawnPrimaryData* GetPawnDataFromController(AController* InController);
	
protected:
	/* override function */
	virtual void BeginPlay() override;

	virtual void InitGameState() override;
	
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	/**/
	void InitPrimaryAssets();
	bool IsPrimaryDataAssetLoaded() const;
	
private:
	void PostPrimaryDataLoaded(const UKLabPrimaryDataAsset* PrimaryData);
	
	void GetPrimaryAssetID(FPrimaryAssetId& OutId, FString& OutSourceName);
	void SetPrimaryAssetsToGameState(FPrimaryAssetId& KLabPrimaryAssetId);
};
