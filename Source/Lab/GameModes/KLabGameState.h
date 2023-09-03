// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "KLabGameState.generated.h"

class UKLabPrimaryAssetManagerComponent;

UCLASS()
class LAB_API AKLabGameState : public AModularGameState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKLabGameState();

	UKLabPrimaryAssetManagerComponent* GetPrimaryAssetComp() const {return PrimaryAssetManager;}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UKLabPrimaryAssetManagerComponent> PrimaryAssetManager;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
