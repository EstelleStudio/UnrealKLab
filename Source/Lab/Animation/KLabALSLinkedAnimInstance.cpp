// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabALSLinkedAnimInstance.h"

#include "KLabALSAnimInstance.h"
#include "Character/KLabALSCharacter.h"

UKLabALSLinkedAnimInstance::UKLabALSLinkedAnimInstance()
{
	RootMotionMode = ERootMotionMode::IgnoreRootMotion;
	bUseMainInstanceMontageEvaluationData = true;
}

void UKLabALSLinkedAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Parent = Cast<UKLabALSAnimInstance>(GetSkelMeshComponent()->GetAnimInstance());
	Character = Cast<AKLabALSCharacter>(GetOwningActor());
}

void UKLabALSLinkedAnimInstance::NativeBeginPlay()
{
	check(Parent.IsValid());
	Super::NativeBeginPlay();
}

FAnimInstanceProxy* UKLabALSLinkedAnimInstance::CreateAnimInstanceProxy()
{
	return new FKLabALSAnimationInstanceProxy{this};
}

void UKLabALSLinkedAnimInstance::ReinitializeLook()
{
	if (Parent.IsValid())
	{
		Parent->ReinitializeLook();
	}
}

void UKLabALSLinkedAnimInstance::RefreshLook()
{
	if (Parent.IsValid())
	{
		Parent->RefreshLook();
	}
}

void UKLabALSLinkedAnimInstance::ResetGroundedEntryMode()
{
	if (Parent.IsValid())
	{
		Parent->ResetGroundedEntryMode();
	}
}

void UKLabALSLinkedAnimInstance::SetHipsDirection(EAlsHipsDirection NewHipsDirection)
{
	if (Parent.IsValid())
	{
		Parent->SetHipsDirection(NewHipsDirection);
	}
}

void UKLabALSLinkedAnimInstance::ActivatePivot()
{
	if (Parent.IsValid())
	{
		Parent->ActivatePivot();
	}
}

void UKLabALSLinkedAnimInstance::ResetJumped()
{
	if (Parent.IsValid())
	{
		Parent->ResetJumped();
	}
}
