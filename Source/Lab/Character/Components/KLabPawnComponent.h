// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "KLabPawnComponent.generated.h"



/**
 * Component that adds functionality to all Pawn classes so it can be used for characters/vehicles/etc.
 * This coordinates the initialization of other components.
 */

class UKLabPawnPrimaryData;

UCLASS()
class LAB_API UKLabPawnComponent : public UPawnComponent
{
	GENERATED_BODY()


	
protected:
	/** Pawn data used to create the pawn. Specified from a spawn function or on a placed instance. */
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<const UKLabPawnPrimaryData> PawnData;
};
