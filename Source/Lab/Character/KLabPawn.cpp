// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPawn.h"

#include "Components/KLabPawnComponent.h"


AKLabPawn::AKLabPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	KLabPawnComponent  = CreateDefaultSubobject<UKLabPawnComponent>(TEXT("KLab Pawn Component"));
}

// Called when the game starts or when spawned
void AKLabPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKLabPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKLabPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

