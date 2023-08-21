// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KLabALSCharacter.generated.h"

class UKLabALSComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class LAB_API AKLabALSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKLabALSCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS")
	TObjectPtr<UKLabALSComponent> ALSComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Input:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_LookMouse;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
