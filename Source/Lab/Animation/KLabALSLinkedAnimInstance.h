// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KLabALSLinkedAnimInstance.generated.h"

class AKLabALSCharacter;
class UKLabALSAnimInstance;

/**
 * 
 */
UCLASS()
class LAB_API UKLabALSLinkedAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	TWeakObjectPtr<UKLabALSAnimInstance> Parent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KLab|ALS", Transient)
	TObjectPtr<AKLabALSCharacter> Character;

public:
	UKLabALSLinkedAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	UFUNCTION(BlueprintPure, Category = "KLab|ALS",
	Meta = (BlueprintProtected, BlueprintThreadSafe, ReturnDisplayName = "Parent"))
	UKLabALSAnimInstance* GetParentUnsafe() const;

	UFUNCTION(BlueprintCallable, Category = "KLab|ALS", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ResetGroundedEntryMode();

	UFUNCTION(BlueprintCallable, Category = "KLab|ALS", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void SetHipsDirection(EAlsHipsDirection NewHipsDirection);

	UFUNCTION(BlueprintCallable, Category = "KLab|ALS", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ActivatePivot();

	UFUNCTION(BlueprintCallable, Category = "KLab|ALS", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ResetJumped();
};

inline UKLabALSAnimInstance* UKLabALSLinkedAnimInstance::GetParentUnsafe() const
{
	return Parent.Get();
}
