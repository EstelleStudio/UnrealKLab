// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"

#include "KLabPlayerController.generated.h"

UCLASS()
class LAB_API AKLabPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKLabPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
