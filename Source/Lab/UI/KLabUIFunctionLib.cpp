// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabUIFunctionLib.h"

#include "GameplayTagContainer.h"
#include "KLabUISubsystem.h"
#include "Base/KLabPrimaryLayout.h"
#include "Common/KLab.h"

bool UKLabUIFunctionLib::PushWidgetToLayerForPlayer(TSoftClassPtr<UKLabActivatableWidget> WidgetClass, FGameplayTag LayerName)
{
	if (UKLabUISubsystem* UISubsystem = UKLabUISubsystem::GetInstance())
	{
		if (WidgetClass.IsNull())
		{
			UE_LOG(LogLab, Error, TEXT("Push widget to layer get null widget class."));
			return false;
		}

		if (UKLabPrimaryLayout* RootLayout = UISubsystem->GetCurrentPrimaryLayout())
		{
			TSharedPtr<FStreamableHandle> StreamingHandle = RootLayout->PushWidgetToLayerStackAsync<UKLabActivatableWidget>(LayerName, WidgetClass);
			// TODO:
		}
	}

	return false;
}
