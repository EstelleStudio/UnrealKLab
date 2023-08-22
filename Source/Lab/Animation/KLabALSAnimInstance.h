// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
// #include "State/AlsControlRigInput.h"
#include "State/AlsFeetState.h"
#include "State/AlsGroundedState.h"
#include "State/AlsInAirState.h"
#include "State/AlsLeanState.h"
#include "State/AlsLocomotionAnimationState.h"
#include "State/AlsMovementBaseState.h"
#include "State/AlsPoseState.h"
#include "State/AlsRotateInPlaceState.h"
#include "State/AlsTransitionsState.h"
#include "State/AlsTurnInPlaceState.h"
#include "State/AlsViewAnimationState.h"
#include "Utility/AlsGameplayTags.h"
#include "KLabALSAnimInstance.generated.h"


class UKLabALSComponent;
class AKLabALSCharacter;
class UAlsAnimationInstanceSettings;
class UKLabALSLinkedAnimInstance;

USTRUCT()
struct LAB_API FKLabALSAnimationInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	friend UKLabALSAnimInstance;
	friend UKLabALSLinkedAnimInstance;

public:
	FKLabALSAnimationInstanceProxy() = default;
	explicit FKLabALSAnimationInstanceProxy(UAnimInstance* AnimationInstance): FAnimInstanceProxy(AnimationInstance) {};
};

/**
 * 
 */
UCLASS()
class LAB_API UKLabALSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	friend UKLabALSLinkedAnimInstance;
	UKLabALSAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KLab|ALS")
	TObjectPtr<UAlsAnimationInstanceSettings> Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	TWeakObjectPtr<AKLabALSCharacter> Character;

	// Used to indicate that the animation instance has not been updated for a long time
	// and its current state may not be correct (such as foot location used in foot locking).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	bool bPendingUpdate{true};

	// Time of the last teleportation event.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient, Meta = (ClampMin = 0))
	float TeleportedTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	TWeakObjectPtr<UKLabALSComponent> ALSComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FAlsLocomotionAnimationState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FAlsGroundedState GroundedState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag LocomotionMode{AlsLocomotionModeTags::Grounded};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag RotationMode{AlsRotationModeTags::ViewDirection};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag Stance{AlsStanceTags::Standing};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag Gait{AlsGaitTags::Walking};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FAlsMovementBaseState MovementBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTag LocomotionAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag GroundedEntryMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FAlsPoseState PoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FAlsViewAnimationState ViewState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FAlsLeanState LeanState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FAlsInAirState InAirState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FAlsFeetState FeetState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FAlsTransitionsState TransitionsState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FAlsRotateInPlaceState RotateInPlaceState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FAlsTurnInPlaceState TurnInPlaceState;
public:
	
	/* override functions */
    virtual void NativeInitializeAnimation() override;
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaTime) override;
    virtual void NativeThreadSafeUpdateAnimation(float DeltaTime) override;
    virtual void NativePostEvaluateAnimation() override;

protected:

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	UFUNCTION(BlueprintPure, Category = "ALS|Als Animation Instance",
		Meta = (BlueprintProtected, BlueprintThreadSafe, ReturnDisplayName = "Setting"))
	UAlsAnimationInstanceSettings* GetSettingsUnsafe() const;

	/*UFUNCTION(BlueprintPure, Category = "ALS|Als Animation Instance",
		Meta = (BlueprintProtected, BlueprintThreadSafe, ReturnDisplayName = "Rig Input"))
	FAlsControlRigInput GetControlRigInput() const;*/

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ReinitializeLook();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void RefreshLook();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ResetGroundedEntryMode();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void SetHipsDirection(EAlsHipsDirection NewHipsDirection);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ActivatePivot();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ResetJumped();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayQuickStopAnimation();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionAnimation(UAnimSequenceBase* Animation, float BlendInDuration = 0.2f, float BlendOutDuration = 0.2f,
								 float PlayRate = 1.0f, float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionLeftAnimation(float BlendInDuration = 0.2f, float BlendOutDuration = 0.2f, float PlayRate = 1.0f,
									 float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionRightAnimation(float BlendInDuration = 0.2f, float BlendOutDuration = 0.2f, float PlayRate = 1.0f,
									  float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void StopTransitionAndTurnInPlaceAnimations(float BlendOutDuration = 0.2f);

private:
	void RefreshViewOnGameThread();
	// movement
	
	void RefreshMovementBaseOnGameThread();

	void RefreshPose();

	void RefreshLocomotionOnGameThread();

	void RefreshGroundedOnGameThread();

	// grounded

	void RefreshGrounded(float DeltaTime);

	void RefreshMovementDirection();

	void RefreshVelocityBlend(float DeltaTime);

	void RefreshRotationYawOffsets();
	
	void RefreshSprint(const FVector3f& RelativeAccelerationAmount, float DeltaTime);

	void RefreshStrideBlendAmount();

	void RefreshWalkRunBlendAmount();

	void RefreshStandingPlayRate();

	void RefreshGroundedLeanAmount(const FVector3f& RelativeAccelerationAmount, float DeltaTime);

	void ResetGroundedLeanAmount(float DeltaTime);

	// air

	void RefreshInAirOnGameThread();

	void RefreshInAir(float DeltaTime);

	void RefreshGroundPredictionAmount();

	void RefreshInAirLeanAmount(float DeltaTime);

	// feet

	void RefreshFeetOnGameThread();

	void RefreshFeet(float DeltaTime);

	void RefreshFoot(FAlsFootState& FootState, const FName& FootIkCurveName, const FName& FootLockCurveName,
					 const FTransform& ComponentTransformInverse, float DeltaTime) const;

	void ProcessFootLockTeleport(FAlsFootState& FootState) const;

	void ProcessFootLockBaseChange(FAlsFootState& FootState, const FTransform& ComponentTransformInverse) const;

	void RefreshFootLock(FAlsFootState& FootState, const FName& FootLockCurveName, const FTransform& ComponentTransformInverse,
						 float DeltaTime, FVector& FinalLocation, FQuat& FinalRotation) const;

	void RefreshFootOffset(FAlsFootState& FootState, float DeltaTime, FVector& FinalLocation, FQuat& FinalRotation) const;

	// Transitions

	void RefreshTransitions();

	void RefreshDynamicTransition();

	void PlayQueuedDynamicTransitionAnimation();

	// rotate
	
	void RefreshRotateInPlace(float DeltaTime);

	// turn
	
	void RefreshTurnInPlace(float DeltaTime);

	void PlayQueuedTurnInPlaceAnimation();

	// view
private:

	void RefreshView(float DeltaTime);

	void RefreshSpineRotation(float DeltaTime);
	
public:
	virtual bool IsSpineRotationAllowed();
	
	void SetGroundedEntryMode(const FGameplayTag& NewGroundedEntryMode);
	
	virtual bool IsRotateInPlaceAllowed();

	virtual bool IsTurnInPlaceAllowed();

	void Jump();
	
	float GetCurveValueClamped01(const FName& CurveName) const;
	
};

inline void UKLabALSAnimInstance::SetGroundedEntryMode(const FGameplayTag& NewGroundedEntryMode)
{
	GroundedEntryMode = NewGroundedEntryMode;
}

inline void UKLabALSAnimInstance::ResetGroundedEntryMode()
{
	GroundedEntryMode = FGameplayTag::EmptyTag;
}

inline void UKLabALSAnimInstance::SetHipsDirection(const EAlsHipsDirection NewHipsDirection)
{
	GroundedState.HipsDirection = NewHipsDirection;
}

inline void UKLabALSAnimInstance::ActivatePivot()
{
	GroundedState.bPivotActivationRequested = true;
}

inline void UKLabALSAnimInstance::Jump()
{
	InAirState.bJumpRequested = true;
}

inline void UKLabALSAnimInstance::ResetJumped()
{
	InAirState.bJumped = false;
}

inline bool UKLabALSAnimInstance::IsSpineRotationAllowed()
{
	return RotationMode == AlsRotationModeTags::Aiming;
}

inline bool UKLabALSAnimInstance::IsRotateInPlaceAllowed()
{
	return RotationMode == AlsRotationModeTags::Aiming;
}

inline bool UKLabALSAnimInstance::IsTurnInPlaceAllowed()
{
	return RotationMode == AlsRotationModeTags::ViewDirection;
}