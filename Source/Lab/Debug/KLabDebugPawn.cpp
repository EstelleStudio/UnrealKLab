// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabDebugPawn.h"


AKLabDebugPawn::AKLabDebugPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

// Called when the game starts or when spawned
void AKLabDebugPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKLabDebugPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKLabDebugPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

