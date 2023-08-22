// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KLabALSCharacter.generated.h"

struct FInputActionValue;
class UKLabALSComponent;
class UInputAction;
class UInputMappingContext;
class UAlsCharacterMovementComponent;

UCLASS()
class LAB_API AKLabALSCharacter : public ACharacter
{
	GENERATED_BODY()
/**************************************************************************************************/	
/************************************* ALS Related Logic ******************************************/
/**************************************************************************************************/
	
public:
	explicit AKLabALSCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual FRotator GetViewRotation() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS")
	TObjectPtr<UKLabALSComponent> ALSComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Als Character")
	TObjectPtr<UAlsCharacterMovementComponent> AlsCharacterMovement;


	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	
/**************************************************************************************************/
/**************************************************************************************************/	
/**************************************************************************************************/

	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Input:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_LookMouse;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
