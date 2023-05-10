// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LabCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class ASceneCaptureCube;
class ALabMovableSceneCapMirror;

UCLASS()
class LAB_API ALabCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALabCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);

	// Input:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> IA_LookMouse;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> IA_LookStick;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Character", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> IA_Jump;

	TObjectPtr<ALabMovableSceneCapMirror> MirrorActor;
	TObjectPtr<ASceneCaptureCube> SceneCaptureCubeActor;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|Character", Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCameraComponent> CameraComponent;

	static const float LookYawRate;
	static const float LookPitchRate;
};
