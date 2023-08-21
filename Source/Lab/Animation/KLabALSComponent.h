// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Utility/AlsGameplayTags.h"
#include "KLabALSComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LAB_API UKLabALSComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKLabALSComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	FGameplayTag LocomotionMode{AlsLocomotionModeTags::Grounded};

	// Locomotion Mode:
public:
	const FGameplayTag& GetLocomotionMode() const;

	// Rotation Mode:
	//const FGameplayTag& GetRotationMode() const;
};

inline const FGameplayTag& UKLabALSComponent::GetLocomotionMode() const
{
	return LocomotionMode;
}