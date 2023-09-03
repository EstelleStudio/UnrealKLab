// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KLabPawn.h"
#include "KLabDebugPawn.generated.h"

UCLASS()
class LAB_API AKLabDebugPawn : public AKLabPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	explicit AKLabDebugPawn(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
