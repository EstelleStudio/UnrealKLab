// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "State/AlsLocomotionState.h"
#include "State/AlsMovementBaseState.h"
#include "State/AlsViewState.h"
#include "Utility/AlsGameplayTags.h"
#include "KLabALSComponent.generated.h"

class UKLabALSAnimInstance;
class UAlsCharacterMovementComponent;
class UAlsCharacterSettings;
class UAlsMovementSettings;
class AKLabALSCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LAB_API UKLabALSComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	explicit UKLabALSComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<AKLabALSCharacter> Owner = nullptr;
	TWeakObjectPtr<UAlsCharacterMovementComponent> AlsCharacterMovement;
	TWeakObjectPtr<UKLabALSAnimInstance> AnimationInstance;
	
// ALS Common:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KLab|ALS")
	TObjectPtr<UAlsCharacterSettings> Settings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KLab|ALS")
	TObjectPtr<UAlsMovementSettings> MovementSettings;

private:
	void RefreshUsingAbsoluteRotation() const;
	void RefreshVisibilityBasedAnimTickOption() const;
	void RefreshMovementBase();

// Timer:
protected:
	FTimerHandle BrakingFrictionFactorResetTimer;
	
// View Mode:
public:
	const FGameplayTag& GetViewMode() const;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated)
	FGameplayTag ViewMode{AlsViewModeTags::ThirdPerson};
	
// Locomotion Mode:
public:
	void OnMovementModeChanged(TEnumAsByte<enum EMovementMode> Mode);
	
	const FGameplayTag& GetLocomotionMode() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag LocomotionMode{AlsLocomotionModeTags::Grounded};

	void SetLocomotionMode(const FGameplayTag& NewLocomotionMode);

private:
	void NotifyLocomotionModeChanged(const FGameplayTag& PreviousLocomotionMode);

// Rotation Mode:
public:
	const FGameplayTag& GetRotationMode() const;

	void SetRotationMode(const FGameplayTag& NewRotationMode);

	void RefreshRotationMode();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag RotationMode{AlsRotationModeTags::ViewDirection};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State")
	bool bDesiredAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated)
	FGameplayTag DesiredRotationMode{AlsRotationModeTags::ViewDirection};

// Stance:
public:
	const FGameplayTag& GetStance() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag Stance{AlsStanceTags::Standing};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated)
	FGameplayTag DesiredStance{AlsStanceTags::Standing};
	
// Gait:
public:
	const FGameplayTag& GetGait() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDesiredGait"))
	void SetDesiredGait(const FGameplayTag& NewDesiredGait);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag Gait{AlsGaitTags::Walking};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated)
	FGameplayTag DesiredGait{AlsGaitTags::Running};

	void SetGait(const FGameplayTag& NewGait);

private:
	void RefreshGait();

	FGameplayTag CalculateMaxAllowedGait() const;

	FGameplayTag CalculateActualGait(const FGameplayTag& MaxAllowedGait) const;

	bool CanSprint() const;

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredGait(const FGameplayTag& NewDesiredGait);

// Locomotion Action
public:
	const FGameplayTag& GetLocomotionAction() const;

	void SetLocomotionAction(const FGameplayTag& NewLocomotionAction);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FGameplayTag LocomotionAction;

// Input
public:
	const FVector& GetInputDirection() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated)
	FVector_NetQuantizeNormal InputDirection;
	void SetInputDirection(FVector NewInputDirection);

	virtual void RefreshInput(float DeltaTime);
	
// Movement Base
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsMovementBaseState MovementBase;

// ViewState
public:
	FRotator GetViewRotation() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsViewState ViewState;

	// Replicated raw view rotation. In most cases, it's better to use FAlsViewState::Rotation to take advantage of network smoothing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		ReplicatedUsing = "OnReplicated_ReplicatedViewRotation")
	FRotator ReplicatedViewRotation;

private:
	void SetReplicatedViewRotation(const FRotator& NewViewRotation);

	UFUNCTION(Server, Unreliable)
	void ServerSetReplicatedViewRotation(const FRotator& NewViewRotation);

	UFUNCTION()
	void OnReplicated_ReplicatedViewRotation();

public:
	void CorrectViewNetworkSmoothing(const FRotator& NewViewRotation);

	const FAlsViewState& GetViewState() const;

private:
	void RefreshView(float DeltaTime);

	void RefreshViewNetworkSmoothing(float DeltaTime);

// Locomotion State
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsLocomotionState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character",
	Transient, Replicated, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float DesiredVelocityYawAngle;

public:
	const FAlsLocomotionState& GetLocomotionState() const;

private:
	void SetDesiredVelocityYawAngle(float NewDesiredVelocityYawAngle);

	void RefreshLocomotionLocationAndRotation();

	void RefreshLocomotionEarly();

	void RefreshLocomotion(float DeltaTime);

	void RefreshLocomotionLate(float DeltaTime);

//  Rotation

private:
	void RefreshGroundedRotation(float DeltaTime);

protected:
	virtual bool RefreshCustomGroundedMovingRotation(float DeltaTime);

	virtual bool RefreshCustomGroundedNotMovingRotation(float DeltaTime);

	void RefreshGroundedMovingAimingRotation(float DeltaTime);

	void RefreshGroundedNotMovingAimingRotation(float DeltaTime);

	float CalculateRotationInterpolationSpeed() const;

private:
	void ApplyRotationYawSpeed(float DeltaTime);

	void RefreshInAirRotation(float DeltaTime);

protected:
	virtual bool RefreshCustomInAirRotation(float DeltaTime);

	void RefreshInAirAimingRotation(float DeltaTime);

	void RefreshRotation(float TargetYawAngle, float DeltaTime, float RotationInterpolationSpeed);

	void RefreshRotationExtraSmooth(float TargetYawAngle, float DeltaTime,
									float RotationInterpolationSpeed, float TargetYawAngleRotationSpeed);

	void RefreshRotationInstant(float TargetYawAngle, ETeleportType Teleport = ETeleportType::None);

	void RefreshTargetYawAngleUsingLocomotionRotation();

	void RefreshTargetYawAngle(float TargetYawAngle);

	void RefreshViewRelativeTargetYawAngle();
	
};

inline const FGameplayTag& UKLabALSComponent::GetLocomotionMode() const
{
	return LocomotionMode;
}