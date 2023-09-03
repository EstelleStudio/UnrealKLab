// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "KLabPlayerState.generated.h"

UCLASS()
class LAB_API AKLabPlayerState : public AModularPlayerState
{
	GENERATED_BODY()

public:
	AKLabPlayerState();

	/* override function */
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void BeginPlay() override;

};
