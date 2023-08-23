// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabALSComponent.h"

#include "AlsCharacterMovementComponent.h"
#include "KLabALSAnimInstance.h"
#include "Character/KLabALSCharacter.h"
#include "Common/KLab.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameNetworkManager.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Settings/AlsCharacterSettings.h"
#include "Utility/AlsConstants.h"

UKLabALSComponent::UKLabALSComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UKLabALSComponent::OnRegister()
{
	RotationMode = bDesiredAiming ? AlsRotationModeTags::Aiming : DesiredRotationMode;
	Stance = DesiredStance;
	Gait = DesiredGait;
	
	Super::OnRegister();
}


// Called when the game starts
void UKLabALSComponent::BeginPlay()
{
	// Make sure the mesh and animation blueprint are ticking after the this component so they can access the most up-to-date character state.

	Owner = Cast<AKLabALSCharacter>(GetOwner());
	check(Owner.IsValid());
	
	AlsCharacterMovement= Owner->AlsCharacterMovement;
	Owner->GetMesh()->AddTickPrerequisiteComponent(this);
	
	SetReplicatedViewRotation(Owner->GetNativeViewRotation().GetNormalized());

	ViewState.NetworkSmoothing.InitialRotation = ReplicatedViewRotation;
	ViewState.NetworkSmoothing.Rotation = ReplicatedViewRotation;
	ViewState.Rotation = ReplicatedViewRotation;
	ViewState.PreviousYawAngle = UE_REAL_TO_FLOAT(ReplicatedViewRotation.Yaw);

	const auto& ActorTransform{Owner->GetActorTransform()};

	LocomotionState.Location = ActorTransform.GetLocation();
	LocomotionState.RotationQuaternion = ActorTransform.GetRotation();
	LocomotionState.Rotation = Owner->GetActorRotation();
	LocomotionState.PreviousYawAngle = UE_REAL_TO_FLOAT(LocomotionState.Rotation.Yaw);

	RefreshTargetYawAngleUsingLocomotionRotation();

	LocomotionState.InputYawAngle = UE_REAL_TO_FLOAT(LocomotionState.Rotation.Yaw);
	LocomotionState.VelocityYawAngle = UE_REAL_TO_FLOAT(LocomotionState.Rotation.Yaw);

	// Pass current movement settings to the movement component.

	AlsCharacterMovement->SetMovementSettings(MovementSettings);

	AnimationInstance = Cast<UKLabALSAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	
	Super::BeginPlay();
	
	RefreshUsingAbsoluteRotation();
	RefreshVisibilityBasedAnimTickOption();

	ViewState.NetworkSmoothing.bEnabled |= IsValid(Settings) && Settings->View.bEnableNetworkSmoothing && GetOwnerRole() == ROLE_SimulatedProxy;

	// Update states to use the initial desired values.

	//RefreshRotationMode();
	AlsCharacterMovement->SetRotationMode(RotationMode);
	//ApplyDesiredStance();
	AlsCharacterMovement->SetStance(Stance);

	RefreshGait();
}


// Called every frame
void UKLabALSComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!IsValid(Settings) || !AnimationInstance.IsValid())
	{
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		return;
	}
	
	RefreshVisibilityBasedAnimTickOption();

	RefreshMovementBase();

	RefreshInput(DeltaTime);

	RefreshLocomotionEarly();

	RefreshView(DeltaTime);

	RefreshRotationMode();

	RefreshLocomotion(DeltaTime);

	RefreshGait();

	RefreshGroundedRotation(DeltaTime);
	RefreshInAirRotation(DeltaTime);

	//TryStartMantlingInAir();

	// RefreshMantling();
	// RefreshRagdolling(DeltaTime);
	// RefreshRolling(DeltaTime);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RefreshLocomotionLate(DeltaTime);

	// if (!GetMesh()->bRecentlyRendered &&
	// 	GetMesh()->VisibilityBasedAnimTickOption > EVisibilityBasedAnimTickOption::AlwaysTickPose)
	// {
	// 	AnimationInstance->MarkPendingUpdate();
	// }

	// TODO:

}

void UKLabALSComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;
	Parameters.Condition = COND_SkipOwner;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredStance, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredGait, Parameters)

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredRotationMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ViewMode, Parameters)
	
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ReplicatedViewRotation, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, InputDirection, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredVelocityYawAngle, Parameters)

}

void UKLabALSComponent::RefreshUsingAbsoluteRotation() const
{
	const auto bNotDedicatedServer{!IsNetMode(NM_DedicatedServer)};
	const auto bAutonomousProxyOnListenServer{IsNetMode(NM_ListenServer) && GetOwnerRole() == ROLE_AutonomousProxy};
	const auto bNonLocallyControllerCharacterWithURO{
		Owner->GetMesh()->ShouldUseUpdateRateOptimizations() && !IsValid(Owner->GetInstigatorController<APlayerController>())
	};
	Owner->GetMesh()->SetUsingAbsoluteRotation(bNotDedicatedServer && (bAutonomousProxyOnListenServer || bNonLocallyControllerCharacterWithURO));
}

void UKLabALSComponent::RefreshVisibilityBasedAnimTickOption() const
{
	// TODO:
	/*const auto DefaultTickOption{GetClass()->GetDefaultObject<AKLabALSCharacter>()->GetMesh()->VisibilityBasedAnimTickOption};

	// Make sure that the pose is always ticked on the server when the character is controlled
	// by a remote client, otherwise some problems may arise (such as jitter when rolling).

	const auto TargetTickOption{
		IsNetMode(NM_Standalone) || GetOwnerRole() <= ROLE_AutonomousProxy || Owner->GetRemoteRole() != ROLE_AutonomousProxy
			? EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered
			: EVisibilityBasedAnimTickOption::AlwaysTickPose
	};

	// Keep the default tick option, at least if the target tick option is not required by the plugin to work properly.

	Owner->GetMesh()->VisibilityBasedAnimTickOption = TargetTickOption <= DefaultTickOption ? TargetTickOption : DefaultTickOption;*/
}

void UKLabALSComponent::RefreshMovementBase()
{
	const FBasedMovementInfo& BasedMovement = Owner->GetBasedMovement();
	if (BasedMovement.MovementBase != MovementBase.Primitive || BasedMovement.BoneName != MovementBase.BoneName)
	{
		MovementBase.Primitive = BasedMovement.MovementBase;
		MovementBase.BoneName = BasedMovement.BoneName;
		MovementBase.bBaseChanged = true;
	}
	else
	{
		MovementBase.bBaseChanged = false;
	}

	MovementBase.bHasRelativeLocation = BasedMovement.HasRelativeLocation();
	MovementBase.bHasRelativeRotation = MovementBase.bHasRelativeLocation && BasedMovement.bRelativeRotation;

	const auto PreviousRotation{MovementBase.Rotation};

	MovementBaseUtility::GetMovementBaseTransform(BasedMovement.MovementBase, BasedMovement.BoneName,
												  MovementBase.Location, MovementBase.Rotation);

	MovementBase.DeltaRotation = MovementBase.bHasRelativeLocation && !MovementBase.bBaseChanged
									 ? (MovementBase.Rotation * PreviousRotation.Inverse()).Rotator()
									 : FRotator::ZeroRotator;

	UE_LOG(LogLab, Log, TEXT("MovementBase Rotation: %ws"), *MovementBase.Rotation.ToString())
	UE_LOG(LogLab, Log, TEXT("MovementBase Delta Rotation: %ws"), *MovementBase.DeltaRotation.ToString())
}

const FGameplayTag& UKLabALSComponent::GetViewMode() const
{
	return ViewMode;
}

void UKLabALSComponent::OnMovementModeChanged(TEnumAsByte<enum EMovementMode> Mode)
{
	switch (Mode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		SetLocomotionMode(AlsLocomotionModeTags::Grounded);
		break;

	case MOVE_Falling:
		SetLocomotionMode(AlsLocomotionModeTags::InAir);
		break;

	default:
		SetLocomotionMode(FGameplayTag::EmptyTag);
		break;
	}
}

void UKLabALSComponent::SetLocomotionMode(const FGameplayTag& NewLocomotionMode)
{
	if (LocomotionMode != NewLocomotionMode)
	{
		const auto PreviousLocomotionMode{LocomotionMode};

		LocomotionMode = NewLocomotionMode;

		NotifyLocomotionModeChanged(PreviousLocomotionMode);
	}
}

void UKLabALSComponent::NotifyLocomotionModeChanged(const FGameplayTag& PreviousLocomotionMode)
{
	if (LocomotionMode == AlsLocomotionModeTags::Grounded &&
	    PreviousLocomotionMode == AlsLocomotionModeTags::InAir)
	{
		static constexpr auto HasInputBrakingFrictionFactor{0.5f};
		static constexpr auto NoInputBrakingFrictionFactor{3.0f};

		Owner->GetCharacterMovement()->BrakingFrictionFactor = LocomotionState.bHasInput
			                                                ? HasInputBrakingFrictionFactor
			                                                : NoInputBrakingFrictionFactor;

		static constexpr auto ResetDelay{0.5f};

		Owner->GetWorldTimerManager().SetTimer(BrakingFrictionFactorResetTimer,
		                                FTimerDelegate::CreateWeakLambda(this, [this]
		                                {
			                                Owner->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		                                }), ResetDelay, false);
		
		LocomotionState.bRotationTowardsLastInputDirectionBlocked = true;
	}
}

const FGameplayTag& UKLabALSComponent::GetRotationMode() const
{
	return RotationMode;
}

void UKLabALSComponent::SetRotationMode(const FGameplayTag& NewRotationMode)
{
	AlsCharacterMovement->SetRotationMode(NewRotationMode);

	if (RotationMode != NewRotationMode)
	{
		RotationMode = NewRotationMode;
	}
}

void UKLabALSComponent::RefreshRotationMode()
{
	const auto bSprinting{Gait == AlsGaitTags::Sprinting};
	const auto bAiming{bDesiredAiming || DesiredRotationMode == AlsRotationModeTags::Aiming};

	if (ViewMode == AlsViewModeTags::FirstPerson)
	{
		if (LocomotionMode == AlsLocomotionModeTags::InAir)
		{
			if (bAiming && Settings->bAllowAimingWhenInAir)
			{
				SetRotationMode(AlsRotationModeTags::Aiming);
			}
			else
			{
				SetRotationMode(AlsRotationModeTags::ViewDirection);
			}

			return;
		}

		// Grounded and other locomotion modes.

		if (bAiming && (!bSprinting || !Settings->bSprintHasPriorityOverAiming))
		{
			SetRotationMode(AlsRotationModeTags::Aiming);
		}
		else
		{
			SetRotationMode(AlsRotationModeTags::ViewDirection);
		}

		return;
	}

	// Third person and other view modes.

	if (LocomotionMode == AlsLocomotionModeTags::InAir)
	{
		if (bAiming && Settings->bAllowAimingWhenInAir)
		{
			SetRotationMode(AlsRotationModeTags::Aiming);
		}
		else if (bAiming)
		{
			SetRotationMode(AlsRotationModeTags::ViewDirection);
		}
		else
		{
			SetRotationMode(DesiredRotationMode);
		}

		return;
	}

	// Grounded and other locomotion modes.

	if (bSprinting)
	{
		if (bAiming && !Settings->bSprintHasPriorityOverAiming)
		{
			SetRotationMode(AlsRotationModeTags::Aiming);
		}
		else if (Settings->bRotateToVelocityWhenSprinting)
		{
			SetRotationMode(AlsRotationModeTags::VelocityDirection);
		}
		else if (bAiming)
		{
			SetRotationMode(AlsRotationModeTags::ViewDirection);
		}
		else
		{
			SetRotationMode(DesiredRotationMode);
		}
	}
	else // Not sprinting.
	{
		if (bAiming)
		{
			SetRotationMode(AlsRotationModeTags::Aiming);
		}
		else
		{
			SetRotationMode(DesiredRotationMode);
		}
	}

	// TODO:
	SetRotationMode(AlsRotationModeTags::Aiming);
}

const FGameplayTag& UKLabALSComponent::GetStance() const
{
	return Stance;
}

const FGameplayTag& UKLabALSComponent::GetGait() const
{
	return Gait;
}

void UKLabALSComponent::SetDesiredGait(const FGameplayTag& NewDesiredGait)
{
	if (DesiredGait != NewDesiredGait)
	{
		DesiredGait = NewDesiredGait;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredGait, this)

		if (GetOwnerRole() == ROLE_AutonomousProxy)
		{
			ServerSetDesiredGait(DesiredGait);
		}
	}
}

void UKLabALSComponent::SetGait(const FGameplayTag& NewGait)
{
	if (Gait != NewGait)
	{
		Gait = NewGait;
	}
}

void UKLabALSComponent::RefreshGait()
{
	if (LocomotionMode != AlsLocomotionModeTags::Grounded)
	{
		return;
	}

	const auto MaxAllowedGait{CalculateMaxAllowedGait()};

	// Update the character max walk speed to the configured speeds based on the currently max allowed gait.

	AlsCharacterMovement->SetMaxAllowedGait(MaxAllowedGait);

	SetGait(CalculateActualGait(MaxAllowedGait));
}

FGameplayTag UKLabALSComponent::CalculateMaxAllowedGait() const
{
	if (DesiredGait != AlsGaitTags::Sprinting)
	{
		return DesiredGait;
	}

	if (CanSprint())
	{
		return AlsGaitTags::Sprinting;
	}

	return AlsGaitTags::Running;
}

FGameplayTag UKLabALSComponent::CalculateActualGait(const FGameplayTag& MaxAllowedGait) const
{
	if (LocomotionState.Speed < AlsCharacterMovement->GetGaitSettings().WalkSpeed + 10.0f)
	{
		return AlsGaitTags::Walking;
	}

	if (LocomotionState.Speed < AlsCharacterMovement->GetGaitSettings().RunSpeed + 10.0f || MaxAllowedGait != AlsGaitTags::Sprinting)
	{
		return AlsGaitTags::Running;
	}

	return AlsGaitTags::Sprinting;
}

bool UKLabALSComponent::CanSprint() const
{
	if (!LocomotionState.bHasInput || Stance != AlsStanceTags::Standing ||
	(RotationMode == AlsRotationModeTags::Aiming && !Settings->bSprintHasPriorityOverAiming))
	{
		return false;
	}

	if (ViewMode != AlsViewModeTags::FirstPerson &&
		(DesiredRotationMode == AlsRotationModeTags::VelocityDirection || Settings->bRotateToVelocityWhenSprinting))
	{
		return true;
	}

	static constexpr auto ViewRelativeAngleThreshold{50.0f};

	if (FMath::Abs(FRotator3f::NormalizeAxis(UE_REAL_TO_FLOAT(
			LocomotionState.InputYawAngle - ViewState.Rotation.Yaw))) < ViewRelativeAngleThreshold)
	{
		return true;
	}

	return false;
}

const FGameplayTag& UKLabALSComponent::GetLocomotionAction() const
{
	return LocomotionAction;
}

void UKLabALSComponent::SetLocomotionAction(const FGameplayTag& NewLocomotionAction)
{
	if (LocomotionAction != NewLocomotionAction)
	{
		LocomotionAction = NewLocomotionAction;
	}
}

const FVector& UKLabALSComponent::GetInputDirection() const
{
	return InputDirection;
}

void UKLabALSComponent::SetInputDirection(FVector NewInputDirection)
{
	NewInputDirection = NewInputDirection.GetSafeNormal();

	COMPARE_ASSIGN_AND_MARK_PROPERTY_DIRTY(ThisClass, InputDirection, NewInputDirection, this);
}

void UKLabALSComponent::RefreshInput(float DeltaTime)
{
	if (GetOwnerRole() >= ROLE_AutonomousProxy)
	{
		SetInputDirection(Owner->GetCharacterMovement()->GetCurrentAcceleration() / static_cast<double>(Owner->GetCharacterMovement()->GetMaxAcceleration()));
	}

	LocomotionState.bHasInput = InputDirection.SizeSquared() > UE_KINDA_SMALL_NUMBER;

	if (LocomotionState.bHasInput)
	{
		LocomotionState.InputYawAngle = UE_REAL_TO_FLOAT(UAlsMath::DirectionToAngleXY(InputDirection));
	}
}

FRotator UKLabALSComponent::GetViewRotation() const
{
	return ViewState.Rotation;
}

void UKLabALSComponent::SetReplicatedViewRotation(const FRotator& NewViewRotation)
{
	if (ReplicatedViewRotation != NewViewRotation)
	{
		ReplicatedViewRotation = NewViewRotation;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, ReplicatedViewRotation, this)

		// The character movement component already sends the view rotation to the
		// server if the movement is replicated, so we don't have to do it ourselves.

		if (!Owner->IsReplicatingMovement() && GetOwnerRole() == ROLE_AutonomousProxy)
		{
			ServerSetReplicatedViewRotation(ReplicatedViewRotation);
		}
	}
}

void UKLabALSComponent::OnReplicated_ReplicatedViewRotation()
{
	CorrectViewNetworkSmoothing(ReplicatedViewRotation);
}

void UKLabALSComponent::CorrectViewNetworkSmoothing(const FRotator& NewViewRotation)
{
	// Based on UCharacterMovementComponent::SmoothCorrection().
	ReplicatedViewRotation = NewViewRotation;
	ReplicatedViewRotation.Normalize();

	auto& NetworkSmoothing{ViewState.NetworkSmoothing};

	if (!NetworkSmoothing.bEnabled)
	{
		NetworkSmoothing.InitialRotation = ReplicatedViewRotation;
		NetworkSmoothing.Rotation = ReplicatedViewRotation;
		return;
	}

	const auto bListenServer{IsNetMode(NM_ListenServer)};

	const auto NewNetworkSmoothingServerTime{
		bListenServer
			? Owner->GetCharacterMovement()->GetServerLastTransformUpdateTimeStamp()
			: Owner->GetReplicatedServerLastTransformUpdateTimeStamp()
	};

	if (NewNetworkSmoothingServerTime <= 0.0f)
	{
		return;
	}

	NetworkSmoothing.InitialRotation = NetworkSmoothing.Rotation;

	// Using server time lets us know how much time elapsed, regardless of packet lag variance.

	const auto ServerDeltaTime{NewNetworkSmoothingServerTime - NetworkSmoothing.ServerTime};

	NetworkSmoothing.ServerTime = NewNetworkSmoothingServerTime;

	// Don't let the client fall too far behind or run ahead of new server time.

	const auto MaxServerDeltaTime{GetDefault<AGameNetworkManager>()->MaxClientSmoothingDeltaTime};

	const auto MinServerDeltaTime{
		FMath::Min(MaxServerDeltaTime, bListenServer
			                               ? Owner->GetCharacterMovement()->ListenServerNetworkSimulatedSmoothLocationTime
			                               : Owner->GetCharacterMovement()->NetworkSimulatedSmoothLocationTime)
	};

	// Calculate how far behind we can be after receiving a new server time.

	const auto MinClientDeltaTime{FMath::Clamp(ServerDeltaTime * 1.25f, MinServerDeltaTime, MaxServerDeltaTime)};

	NetworkSmoothing.ClientTime = FMath::Clamp(NetworkSmoothing.ClientTime,
	                                           NetworkSmoothing.ServerTime - MinClientDeltaTime,
	                                           NetworkSmoothing.ServerTime);

	// Compute actual delta between new server time and client simulation.

	NetworkSmoothing.Duration = NetworkSmoothing.ServerTime - NetworkSmoothing.ClientTime;
}

const FAlsViewState& UKLabALSComponent::GetViewState() const
{
	return ViewState;
}

void UKLabALSComponent::RefreshView(float DeltaTime)
{
	if (MovementBase.bHasRelativeRotation)
	{
		// Offset the rotations to keep them relative to the movement base.

		ReplicatedViewRotation.Pitch += MovementBase.DeltaRotation.Pitch;
		ReplicatedViewRotation.Yaw += MovementBase.DeltaRotation.Yaw;
		ReplicatedViewRotation.Normalize();

		ViewState.Rotation.Pitch += MovementBase.DeltaRotation.Pitch;
		ViewState.Rotation.Yaw += MovementBase.DeltaRotation.Yaw;
		ViewState.Rotation.Normalize();
	}

	ViewState.PreviousYawAngle = UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw);

	if ((Owner->IsReplicatingMovement() && GetOwnerRole() >= ROLE_AutonomousProxy) || Owner->IsLocallyControlled())
	{
		SetReplicatedViewRotation(Owner->GetNativeViewRotation().GetNormalized());
	}

	RefreshViewNetworkSmoothing(DeltaTime);

	ViewState.Rotation = ViewState.NetworkSmoothing.Rotation;

	// Set the yaw speed by comparing the current and previous view yaw angle, divided by
	// delta seconds. This represents the speed the camera is rotating from left to right.

	ViewState.YawSpeed = FMath::Abs(UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw - ViewState.PreviousYawAngle)) / DeltaTime;

	UE_LOG(LogLab, Log, TEXT("View Rotation: %ws"), *ViewState.Rotation.ToString());
}

void UKLabALSComponent::RefreshViewNetworkSmoothing(float DeltaTime)
{
	// Based on UCharacterMovementComponent::SmoothClientPosition_Interpolate()
	// and UCharacterMovementComponent::SmoothClientPosition_UpdateVisuals().

	auto& NetworkSmoothing{ViewState.NetworkSmoothing};

	if (!NetworkSmoothing.bEnabled ||
		NetworkSmoothing.ClientTime >= NetworkSmoothing.ServerTime ||
		NetworkSmoothing.Duration <= UE_SMALL_NUMBER)
	{
		NetworkSmoothing.InitialRotation = ReplicatedViewRotation;
		NetworkSmoothing.Rotation = ReplicatedViewRotation;
		return;
	}

	if (MovementBase.bHasRelativeRotation)
	{
		// Offset the rotations to keep them relative to the movement base.

		NetworkSmoothing.InitialRotation.Pitch += MovementBase.DeltaRotation.Pitch;
		NetworkSmoothing.InitialRotation.Yaw += MovementBase.DeltaRotation.Yaw;
		NetworkSmoothing.InitialRotation.Normalize();

		NetworkSmoothing.Rotation.Pitch += MovementBase.DeltaRotation.Pitch;
		NetworkSmoothing.Rotation.Yaw += MovementBase.DeltaRotation.Yaw;
		NetworkSmoothing.Rotation.Normalize();
	}

	NetworkSmoothing.ClientTime += DeltaTime;

	const auto InterpolationAmount{
		UAlsMath::Clamp01(1.0f - (NetworkSmoothing.ServerTime - NetworkSmoothing.ClientTime) / NetworkSmoothing.Duration)
	};

	if (!FAnimWeight::IsFullWeight(InterpolationAmount))
	{
		NetworkSmoothing.Rotation = UAlsMath::LerpRotator(NetworkSmoothing.InitialRotation, ReplicatedViewRotation, InterpolationAmount);
	}
	else
	{
		NetworkSmoothing.ClientTime = NetworkSmoothing.ServerTime;
		NetworkSmoothing.Rotation = ReplicatedViewRotation;
	}
}

const FAlsLocomotionState& UKLabALSComponent::GetLocomotionState() const
{
	return LocomotionState;
}

void UKLabALSComponent::SetDesiredVelocityYawAngle(float NewDesiredVelocityYawAngle)
{
	COMPARE_ASSIGN_AND_MARK_PROPERTY_DIRTY(ThisClass, DesiredVelocityYawAngle, NewDesiredVelocityYawAngle, this);
}

void UKLabALSComponent::RefreshLocomotionLocationAndRotation()
{
	const auto& ActorTransform{Owner->GetActorTransform()};

	// If network smoothing is disabled, then return regular actor transform.

	if (Owner->GetCharacterMovement()->NetworkSmoothingMode == ENetworkSmoothingMode::Disabled)
	{
		LocomotionState.Location = ActorTransform.GetLocation();
		LocomotionState.RotationQuaternion = ActorTransform.GetRotation();
		LocomotionState.Rotation = Owner->GetActorRotation();
	}
	else if (Owner->GetMesh()->IsUsingAbsoluteRotation())
	{
		LocomotionState.Location = ActorTransform.TransformPosition(Owner->GetMesh()->GetRelativeLocation() - Owner->GetBaseTranslationOffset());
		LocomotionState.RotationQuaternion = ActorTransform.GetRotation();
		LocomotionState.Rotation = Owner->GetActorRotation();
	}
	else
	{
		const auto SmoothTransform{
			ActorTransform * FTransform{
				Owner->GetMesh()->GetRelativeRotationCache().RotatorToQuat(Owner->GetMesh()->GetRelativeRotation()) * Owner->GetBaseRotationOffset().Inverse(),
				Owner->GetMesh()->GetRelativeLocation() - Owner->GetBaseTranslationOffset()
			}
		};

		LocomotionState.Location = SmoothTransform.GetLocation();
		LocomotionState.RotationQuaternion = SmoothTransform.GetRotation();
		LocomotionState.Rotation = LocomotionState.RotationQuaternion.Rotator();
	}
}

void UKLabALSComponent::RefreshLocomotionEarly()
{
	if (MovementBase.bHasRelativeRotation)
	{
		// Offset the rotations (the actor's rotation too) to keep them relative to the movement base.

		LocomotionState.TargetYawAngle = FRotator3f::NormalizeAxis(LocomotionState.TargetYawAngle +
																   MovementBase.DeltaRotation.Yaw);

		LocomotionState.ViewRelativeTargetYawAngle = FRotator3f::NormalizeAxis(LocomotionState.ViewRelativeTargetYawAngle +
																			   MovementBase.DeltaRotation.Yaw);

		LocomotionState.SmoothTargetYawAngle = FRotator3f::NormalizeAxis(LocomotionState.SmoothTargetYawAngle +
																		 MovementBase.DeltaRotation.Yaw);

		auto NewRotation{Owner->GetActorRotation()};
		NewRotation.Pitch += MovementBase.DeltaRotation.Pitch;
		NewRotation.Yaw += MovementBase.DeltaRotation.Yaw;
		NewRotation.Normalize();

		Owner->SetActorRotation(NewRotation);
	}

	RefreshLocomotionLocationAndRotation();

	LocomotionState.PreviousVelocity = LocomotionState.Velocity;
	LocomotionState.PreviousYawAngle = UE_REAL_TO_FLOAT(LocomotionState.Rotation.Yaw);
}

void UKLabALSComponent::RefreshLocomotion(float DeltaTime)
{
	LocomotionState.Velocity = Owner->GetVelocity();

	// Determine if the character is moving by getting its speed. The speed equals the length
	// of the horizontal velocity, so it does not take vertical movement into account. If the
	// character is moving, update the last velocity rotation. This value is saved because it might
	// be useful to know the last orientation of a movement even after the character has stopped.

	LocomotionState.Speed = UE_REAL_TO_FLOAT(LocomotionState.Velocity.Size2D());

	static constexpr auto HasSpeedThreshold{1.0f};

	LocomotionState.bHasSpeed = LocomotionState.Speed >= HasSpeedThreshold;

	if (LocomotionState.bHasSpeed)
	{
		LocomotionState.VelocityYawAngle = UE_REAL_TO_FLOAT(UAlsMath::DirectionToAngleXY(LocomotionState.Velocity));
	}

	if (Settings->bRotateTowardsDesiredVelocityInVelocityDirectionRotationMode && Owner->GetLocalRole() >= ROLE_AutonomousProxy)
	{
		FVector DesiredVelocity;

		SetDesiredVelocityYawAngle(AlsCharacterMovement->TryConsumePrePenetrationAdjustmentVelocity(DesiredVelocity) &&
								   DesiredVelocity.Size2D() >= HasSpeedThreshold
									   ? UE_REAL_TO_FLOAT(UAlsMath::DirectionToAngleXY(DesiredVelocity))
									   : LocomotionState.VelocityYawAngle);
	}

	LocomotionState.Acceleration = (LocomotionState.Velocity - LocomotionState.PreviousVelocity) / static_cast<double>(DeltaTime);

	// Character is moving if has speed and current acceleration, or if the speed is greater than the moving speed threshold.

	LocomotionState.bMoving = (LocomotionState.bHasInput && LocomotionState.bHasSpeed) ||
							  LocomotionState.Speed > Settings->MovingSpeedThreshold;
}

void UKLabALSComponent::RefreshLocomotionLate(float DeltaTime)
{
	if (!LocomotionMode.IsValid() || LocomotionAction.IsValid())
	{
		RefreshLocomotionLocationAndRotation();
		RefreshTargetYawAngleUsingLocomotionRotation();
	}

	LocomotionState.YawSpeed = FRotator3f::NormalizeAxis(UE_REAL_TO_FLOAT(
								   LocomotionState.Rotation.Yaw - LocomotionState.PreviousYawAngle)) / DeltaTime;
}

void UKLabALSComponent::RefreshGroundedRotation(float DeltaTime)
{
	if (LocomotionAction.IsValid() || LocomotionMode != AlsLocomotionModeTags::Grounded)
	{
		return;
	}

	if (Owner->HasAnyRootMotion())
	{
		RefreshTargetYawAngleUsingLocomotionRotation();
		return;
	}

	if (!LocomotionState.bMoving)
	{
		// Not moving.

		ApplyRotationYawSpeed(DeltaTime);

		if (RefreshCustomGroundedNotMovingRotation(DeltaTime))
		{
			return;
		}

		if (RotationMode == AlsRotationModeTags::Aiming || ViewMode == AlsViewModeTags::FirstPerson)
		{
			RefreshGroundedNotMovingAimingRotation(DeltaTime);
			return;
		}

		if (RotationMode == AlsRotationModeTags::VelocityDirection)
		{
			// Rotate to the last target yaw angle when not moving (relative to the movement base or not).

			const auto TargetYawAngle{
				MovementBase.bHasRelativeLocation && !MovementBase.bHasRelativeRotation &&
				Settings->bInheritMovementBaseRotationInVelocityDirectionRotationMode
					? FRotator3f::NormalizeAxis(LocomotionState.TargetYawAngle + MovementBase.DeltaRotation.Yaw)
					: LocomotionState.TargetYawAngle
			};

			static constexpr auto RotationInterpolationSpeed{12.0f};
			static constexpr auto TargetYawAngleRotationSpeed{800.0f};

			RefreshRotationExtraSmooth(TargetYawAngle, DeltaTime, RotationInterpolationSpeed, TargetYawAngleRotationSpeed);
			return;
		}

		RefreshTargetYawAngleUsingLocomotionRotation();
		return;
	}

	// Moving.

	if (RefreshCustomGroundedMovingRotation(DeltaTime))
	{
		return;
	}

	if (RotationMode == AlsRotationModeTags::VelocityDirection &&
	    (LocomotionState.bHasInput || !LocomotionState.bRotationTowardsLastInputDirectionBlocked))
	{
		LocomotionState.bRotationTowardsLastInputDirectionBlocked = false;

		static constexpr auto TargetYawAngleRotationSpeed{800.0f};

		RefreshRotationExtraSmooth(
			Settings->bRotateTowardsDesiredVelocityInVelocityDirectionRotationMode
				? DesiredVelocityYawAngle
				: LocomotionState.VelocityYawAngle,
			DeltaTime, CalculateRotationInterpolationSpeed(), TargetYawAngleRotationSpeed);
		return;
	}

	if (RotationMode == AlsRotationModeTags::ViewDirection)
	{
		const auto TargetYawAngle{
			Gait == AlsGaitTags::Sprinting
				? LocomotionState.VelocityYawAngle
				: UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw +
					Owner->GetMesh()->GetAnimInstance()->GetCurveValue(UAlsConstants::RotationYawOffsetCurveName()))
		};

		static constexpr auto TargetYawAngleRotationSpeed{500.0f};

		RefreshRotationExtraSmooth(TargetYawAngle, DeltaTime, CalculateRotationInterpolationSpeed(), TargetYawAngleRotationSpeed);
		return;
	}

	if (RotationMode == AlsRotationModeTags::Aiming)
	{
		RefreshGroundedMovingAimingRotation(DeltaTime);
		return;
	}

	RefreshTargetYawAngleUsingLocomotionRotation();
}

bool UKLabALSComponent::RefreshCustomGroundedMovingRotation(float DeltaTime)
{
	return false;
}

bool UKLabALSComponent::RefreshCustomGroundedNotMovingRotation(float DeltaTime)
{
	return false;
}

void UKLabALSComponent::RefreshGroundedMovingAimingRotation(float DeltaTime)
{
	static constexpr auto RotationInterpolationSpeed{20.0f};
	static constexpr auto TargetYawAngleRotationSpeed{1000.0f};

	RefreshRotationExtraSmooth(UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw), DeltaTime,
							   RotationInterpolationSpeed, TargetYawAngleRotationSpeed);
}

void UKLabALSComponent::RefreshGroundedNotMovingAimingRotation(float DeltaTime)
{
	static constexpr auto RotationInterpolationSpeed{20.0f};

	if (LocomotionState.bHasInput)
	{
		static constexpr auto TargetYawAngleRotationSpeed{1000.0f};

		RefreshRotationExtraSmooth(UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw), DeltaTime,
								   RotationInterpolationSpeed, TargetYawAngleRotationSpeed);
		return;
	}

	// Prevent the character from rotating past a certain angle.

	auto ViewRelativeYawAngle{FRotator3f::NormalizeAxis(UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw - LocomotionState.Rotation.Yaw))};

	static constexpr auto ViewRelativeYawAngleThreshold{70.0f};

	if (FMath::Abs(ViewRelativeYawAngle) > ViewRelativeYawAngleThreshold)
	{
		if (ViewRelativeYawAngle > 180.0f - UAlsMath::CounterClockwiseRotationAngleThreshold)
		{
			ViewRelativeYawAngle -= 360.0f;
		}

		RefreshRotation(FRotator3f::NormalizeAxis(UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw +
							(ViewRelativeYawAngle >= 0.0f ? -ViewRelativeYawAngleThreshold : ViewRelativeYawAngleThreshold))),
						DeltaTime, RotationInterpolationSpeed);
	}
	else
	{
		RefreshTargetYawAngleUsingLocomotionRotation();
	}
}

float UKLabALSComponent::CalculateRotationInterpolationSpeed() const
{
	// Calculate the rotation speed by using the rotation speed curve in the movement gait settings. Using
	// the curve in conjunction with the gait amount gives you a high level of control over the rotation
	// rates for each speed. Increase the speed if the camera is rotating quickly for more responsive rotation.

	const auto* RotationInterpolationSpeedCurve{AlsCharacterMovement->GetGaitSettings().RotationInterpolationSpeedCurve.Get()};

	static constexpr auto DefaultRotationInterpolationSpeed{5.0f};

	const auto RotationInterpolationSpeed{
		IsValid(RotationInterpolationSpeedCurve)
			? RotationInterpolationSpeedCurve->GetFloatValue(AlsCharacterMovement->CalculateGaitAmount())
			: DefaultRotationInterpolationSpeed
	};

	static constexpr auto MaxInterpolationSpeedMultiplier{3.0f};
	static constexpr auto ReferenceViewYawSpeed{300.0f};

	return RotationInterpolationSpeed *
		   UAlsMath::LerpClamped(1.0f, MaxInterpolationSpeedMultiplier, ViewState.YawSpeed / ReferenceViewYawSpeed);
}

void UKLabALSComponent::ApplyRotationYawSpeed(float DeltaTime)
{
	const auto DeltaYawAngle{Owner->GetMesh()->GetAnimInstance()->GetCurveValue(UAlsConstants::RotationYawSpeedCurveName()) * DeltaTime};
	if (FMath::Abs(DeltaYawAngle) > UE_SMALL_NUMBER)
	{
		auto NewRotation{Owner->GetActorRotation()};
		NewRotation.Yaw += DeltaYawAngle;

		Owner->SetActorRotation(NewRotation);

		RefreshLocomotionLocationAndRotation();
		RefreshTargetYawAngleUsingLocomotionRotation();
	}
}

void UKLabALSComponent::RefreshInAirRotation(float DeltaTime)
{
	if (LocomotionAction.IsValid() || LocomotionMode != AlsLocomotionModeTags::InAir)
	{
		return;
	}

	if (RefreshCustomInAirRotation(DeltaTime))
	{
		return;
	}

	static constexpr auto RotationInterpolationSpeed{5.0f};

	if (RotationMode == AlsRotationModeTags::VelocityDirection || RotationMode == AlsRotationModeTags::ViewDirection)
	{
		switch (Settings->InAirRotationMode)
		{
		case EAlsInAirRotationMode::RotateToVelocityOnJump:
			if (LocomotionState.bMoving)
			{
				RefreshRotation(LocomotionState.VelocityYawAngle, DeltaTime, RotationInterpolationSpeed);
			}
			else
			{
				RefreshTargetYawAngleUsingLocomotionRotation();
			}
			break;

		case EAlsInAirRotationMode::KeepRelativeRotation:
			RefreshRotation(FRotator3f::NormalizeAxis(UE_REAL_TO_FLOAT(
								ViewState.Rotation.Yaw - LocomotionState.ViewRelativeTargetYawAngle)),
							DeltaTime, RotationInterpolationSpeed);
			break;

		default:
			RefreshTargetYawAngleUsingLocomotionRotation();
			break;
		}
	}
	else if (RotationMode == AlsRotationModeTags::Aiming)
	{
		RefreshInAirAimingRotation(DeltaTime);
	}
	else
	{
		RefreshTargetYawAngleUsingLocomotionRotation();
	}
}

bool UKLabALSComponent::RefreshCustomInAirRotation(float DeltaTime)
{
	return false;
}

void UKLabALSComponent::RefreshInAirAimingRotation(float DeltaTime)
{
	static constexpr auto RotationInterpolationSpeed{15.0f};

	RefreshRotation(UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw), DeltaTime, RotationInterpolationSpeed);
}

void UKLabALSComponent::RefreshRotation(float TargetYawAngle, float DeltaTime, float RotationInterpolationSpeed)
{
	RefreshTargetYawAngle(TargetYawAngle);

	auto NewRotation{Owner->GetActorRotation()};
	NewRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(NewRotation.Yaw)),
													  TargetYawAngle, DeltaTime, RotationInterpolationSpeed);

	Owner->SetActorRotation(NewRotation);

	RefreshLocomotionLocationAndRotation();
}

void UKLabALSComponent::RefreshRotationExtraSmooth(float TargetYawAngle, float DeltaTime, float RotationInterpolationSpeed, float TargetYawAngleRotationSpeed)
{
	LocomotionState.TargetYawAngle = TargetYawAngle;

	RefreshViewRelativeTargetYawAngle();

	// Interpolate target yaw angle for extra smooth rotation.

	LocomotionState.SmoothTargetYawAngle = UAlsMath::InterpolateAngleConstant(LocomotionState.SmoothTargetYawAngle, TargetYawAngle,
																			  DeltaTime, TargetYawAngleRotationSpeed);

	auto NewRotation{Owner->GetActorRotation()};
	NewRotation.Yaw = UAlsMath::ExponentialDecayAngle(UE_REAL_TO_FLOAT(FRotator::NormalizeAxis(NewRotation.Yaw)),
													  LocomotionState.SmoothTargetYawAngle, DeltaTime, RotationInterpolationSpeed);

	Owner->SetActorRotation(NewRotation);

	RefreshLocomotionLocationAndRotation();
}

void UKLabALSComponent::RefreshRotationInstant(float TargetYawAngle, ETeleportType Teleport)
{
	RefreshTargetYawAngle(TargetYawAngle);

	auto NewRotation{Owner->GetActorRotation()};
	NewRotation.Yaw = TargetYawAngle;

	Owner->SetActorRotation(NewRotation, Teleport);

	RefreshLocomotionLocationAndRotation();
}

void UKLabALSComponent::RefreshTargetYawAngleUsingLocomotionRotation()
{
	RefreshTargetYawAngle(UE_REAL_TO_FLOAT(LocomotionState.Rotation.Yaw));
}

void UKLabALSComponent::RefreshTargetYawAngle(float TargetYawAngle)
{
	LocomotionState.TargetYawAngle = TargetYawAngle;

	RefreshViewRelativeTargetYawAngle();

	LocomotionState.SmoothTargetYawAngle = TargetYawAngle;
}

void UKLabALSComponent::RefreshViewRelativeTargetYawAngle()
{
	LocomotionState.ViewRelativeTargetYawAngle = FRotator3f::NormalizeAxis(UE_REAL_TO_FLOAT(
	ViewState.Rotation.Yaw - LocomotionState.TargetYawAngle));
}

void UKLabALSComponent::ServerSetReplicatedViewRotation_Implementation(const FRotator& NewViewRotation)
{
	SetReplicatedViewRotation(NewViewRotation);
}


void UKLabALSComponent::ServerSetDesiredGait_Implementation(const FGameplayTag& NewDesiredGait)
{
	SetDesiredGait(NewDesiredGait);
}


