// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KLabHUD.generated.h"

UCLASS()
class LAB_API AKLabHUD : public AHUD
{
	GENERATED_BODY()

public:
	AKLabHUD();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
