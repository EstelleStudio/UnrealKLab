// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPawnComponent.h"
#include "Character/KLabPawnPrimaryData.h"
#include "Net/UnrealNetwork.h"

UKLabPawnComponent::UKLabPawnComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PawnData = nullptr;
}

void UKLabPawnComponent::SetPawnData(const UKLabPawnPrimaryData* InPawnData)
{
	// initialized when game mode spawn default pawn, game mode get it from controller, controller get it from player state.
	check(IsValid(InPawnData))
	
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	PawnData = InPawnData;
	if (APawn* Owner = GetPawnChecked<APawn>())
	{
		Owner->ForceNetUpdate();
	}
	CheckDefaultInitialization();
}

UKLabPawnComponent* UKLabPawnComponent::GetFromOwner(const AActor* Owner) 
{
	return Owner ? Owner->FindComponentByClass<UKLabPawnComponent>() : nullptr;
}

void UKLabPawnComponent::OnRegister()
{
	Super::OnRegister();
	
	if (!CheckValid())
	{
		ensureAlwaysMsgf(false, TEXT("Pawn component is not valid."));
	}
	// TODO: Register init state game feature		
}

void UKLabPawnComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

bool UKLabPawnComponent::CheckValid()
{
	const APawn* Owner = GetPawn<APawn>();
	return (Owner != nullptr);
}

void UKLabPawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKLabPawnComponent, PawnData);
}
