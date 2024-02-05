// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabUIFunctionLib.h"

#include "GameplayTagContainer.h"
#include "KLabUISubsystem.h"
#include "Base/KLabPrimaryLayout.h"
#include "Common/KLab.h"

UKLabActivatableWidget* UKLabUIFunctionLib::PushWidgetToLayerForPlayer(TSubclassOf<UKLabActivatableWidget> WidgetClass, FGameplayTag LayerName)
{
	if (UKLabUISubsystem* UISubsystem = UKLabUISubsystem::GetInstance())
	{
		if (WidgetClass == nullptr)
		{
			UE_LOG(LogLab, Error, TEXT("Push widget to layer get null widget class."));
			return nullptr;
		}

		if (UKLabPrimaryLayout* RootLayout = UISubsystem->GetCurrentPrimaryLayout())
		{
			return RootLayout->PushWidgetToLayerStack<UKLabActivatableWidget>(LayerName, WidgetClass);
		}
	}

	return nullptr;
}
