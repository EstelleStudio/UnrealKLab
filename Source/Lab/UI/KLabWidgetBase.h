// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KLabWidgetBase.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = UI, meta = (Category = "KLabUI", DisableNativeTick))
class LAB_API UKLabWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	template <typename PlayerControllerT = APlayerController>
	PlayerControllerT& GetOwningPlayerChecked() const
	{
		PlayerControllerT* PC = GetOwningPlayer<PlayerControllerT>();
		check(PC);
		return *PC;
	}

	TSharedPtr<FSlateUser> GetOwnerSlateUser() const;
};
