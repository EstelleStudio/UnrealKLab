// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"

#include "KLabGameMode.generated.h"

UCLASS()
class LAB_API AKLabGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKLabGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
