// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabActivatableWidget.h"

#include "Common/KLab.h"

void UKLabActivatableWidget::ActivateWidget()
{
	if (!bIsActive)
	{
		UE_LOG(LogLab, Log, TEXT("[%s] widget -> Activated"), *GetName());
		bIsActive = true;
		NativeOnActivated();
	}
}

void UKLabActivatableWidget::DeactivateWidget()
{
	if (bIsActive)
	{
		UE_LOG(LogLab, Log, TEXT("[%s] widget -> Deactivated"), *GetName());
		bIsActive = false;
		NativeOnDeactivated();
	}	
}

void UKLabActivatableWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (bAutoActivate)
	{
		UE_LOG(LogLab, Log, TEXT("[%s] Widget auto-activated"), *GetName());
		ActivateWidget();
	}
}

void UKLabActivatableWidget::NativeDestruct()
{
	if (UGameInstance* GameInstance = GetGameInstance<UGameInstance>())
	{
		// Deactivations might rely on members of the game instance to validly run.
		// If there's no game instance, any cleanup done in Deactivation will be irrelevant; we're shutting down the game
		DeactivateWidget();
	}
	Super::NativeDestruct();
}

void UKLabActivatableWidget::NativeOnActivated()
{
	if (ensureMsgf(bIsActive, TEXT("[%s] has called NativeOnActivated, but isn't actually activated! Never call this directly - call ActivateWidget()")))
	{
		if (bSetVisibilityOnActivated)
		{
			SetVisibility(ActivatedVisibility);
		}

		OnActivated().Broadcast();
	}
}

void UKLabActivatableWidget::NativeOnDeactivated()
{
	if (ensure(!bIsActive))
	{
		if (bSetVisibilityOnDeactivated)
		{
			SetVisibility(DeactivatedVisibility);
		}
		OnDeactivated().Broadcast();
	}
}
