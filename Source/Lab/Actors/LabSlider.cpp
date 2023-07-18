// Fill out your copyright notice in the Description page of Project Settings.


#include "LabSlider.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALabSlider::ALabSlider()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	// Init Component
	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = RootComp; 

	SliderBaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	SliderBaseMeshComp->SetupAttachment(RootComp);

	SliderLeftCapMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftCap"));
	SliderLeftCapMeshComp->SetupAttachment(RootComp);

	SliderRightCapMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightCap"));
	SliderRightCapMeshComp->SetupAttachment(RootComp);

	SliderMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slider"));
	SliderMeshComp->SetupAttachment(RootComp);

	// Init Slider 
	InitSliderPosition();
	InitSliderBaseRotationAndScale();

	// Init Text Markers:
	if (bDrawText)
	{
		AddTextMarkers();
	}
}

// Called when the game starts or when spawned
void ALabSlider::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALabSlider::OnRep_CurrentValue()
{
}

// Called every frame
void ALabSlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALabSlider::InitSliderPosition()
{

}

void ALabSlider::InitSliderBaseRotationAndScale()
{
}

void ALabSlider::AddTextMarkers()
{
}

void ALabSlider::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALabSlider, CurrentValue);
}