// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPawn.h"
#include "KLabPawn.generated.h"

class UKLabPawnComponent;

UCLASS()
class LAB_API AKLabPawn : public AModularPawn
{
	GENERATED_BODY()
	
public:
	explicit AKLabPawn(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|Pawn", meta = (AllowPrivateAccess = true))
	TObjectPtr<UKLabPawnComponent> KLabPawnComponent;
};
