// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KLabWidgetBase.h"
#include "KLabActivatableWidget.generated.h"

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class LAB_API UKLabActivatableWidget : public UKLabWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	bool IsActivated() const { return bIsActive; }

	UFUNCTION(BlueprintCallable)
	void ActivateWidget();

	UFUNCTION(BlueprintCallable)
	void DeactivateWidget();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeOnActivated();
	virtual void NativeOnDeactivated();
	
	UPROPERTY(EditAnywhere)
	bool bAutoActivate = false;
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsActive = false;
	
};
