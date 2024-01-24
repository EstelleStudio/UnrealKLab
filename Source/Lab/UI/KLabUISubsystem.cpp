#include "KLabUISubsystem.h"

#include "Base/KLabPrimaryLayout.h"
#include "Common/KLab.h"
#include "Common/KLabGameSettings.h"
#include "Player/KLabLocalPlayer.h"

namespace KLabUISubsystemHelper
{
	constexpr int32 PrimaryLayoutZOrder = 1000;
}

void UKLabUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UKLabGameSettings* GameSettings = GetDefault<UKLabGameSettings>())
	{
		LayoutClass = GameSettings->LayoutClass;
	}
	
	// Different from Lyra here, we bind delegates to default game instance for PrimaryLayout lifetime. 
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		GameInstance->OnLocalPlayerAddedEvent.AddUObject(this, &ThisClass::NotifyPlayerAdded);
		GameInstance->OnLocalPlayerRemovedEvent.AddUObject(this, &ThisClass::NotifyPlayerRemoved);
	}
}

void UKLabUISubsystem::Deinitialize()
{
	Super::Deinitialize();
}

/*
 * don't create ui system on dedicated server
 */
bool UKLabUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		// Only create an instance if there is no override implementation defined elsewhere
		return ChildClasses.Num() == 0;
	}
	return false;	
}

UWorld* UKLabUISubsystem::GetWorld() const
{
	if (GetGameInstance())
	{
		return GetGameInstance()->GetWorld();
	}
	return nullptr;
}

void UKLabUISubsystem::NotifyPlayerAdded(ULocalPlayer* InLocalPlayer)
{
	if (UKLabLocalPlayer* LocalPlayer = Cast<UKLabLocalPlayer>(InLocalPlayer))
	{
		// Player controller of LocalPlayer is null here, dont create widget directly here, using delegate ~~.
		LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this](UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
		{
			RemovePrimaryLayoutWidget(LocalPlayer);
			CreatePrimaryLayoutWidget(LocalPlayer);
		});
	}
}

void UKLabUISubsystem::NotifyPlayerRemoved(ULocalPlayer* InLocalPlayer)
{
	if (UKLabLocalPlayer* LocalPlayer = Cast<UKLabLocalPlayer>(InLocalPlayer))
	{
		RemovePrimaryLayoutWidget(LocalPlayer);
		LocalPlayer->OnPlayerControllerSet.RemoveAll(this);
	}
}

void UKLabUISubsystem::CreatePrimaryLayoutWidget(ULocalPlayer* InLocalPlayer)
{
	if (!CurrentPrimaryLayout)
	{
		if (!LayoutClass.IsNull())
		{
			if (APlayerController* PlayerController = InLocalPlayer->GetPlayerController(GetWorld()))
			{
				TSubclassOf<UKLabPrimaryLayout> LayoutAssetClass = LayoutClass.LoadSynchronous();
				if (LayoutAssetClass && !LayoutAssetClass->HasAnyClassFlags(CLASS_Abstract))
				{
					CurrentPrimaryLayout = CreateWidget<UKLabPrimaryLayout>(PlayerController, LayoutAssetClass);
					CurrentPrimaryLayout->SetPlayerContext(FLocalPlayerContext(InLocalPlayer));
					CurrentPrimaryLayout->AddToPlayerScreen(KLabUISubsystemHelper::PrimaryLayoutZOrder);
					UE_LOG(LogLab, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(InLocalPlayer), *GetNameSafe(CurrentPrimaryLayout));
				}	
			}
		}
	}
	else 
	{
		UE_LOG(LogLab, Warning, TEXT("Current primary layout UI is not nullptr when a local player added. Check!"));
	}
}

void UKLabUISubsystem::RemovePrimaryLayoutWidget(ULocalPlayer* InLocalPlayer)
{
	if (CurrentPrimaryLayout)
	{
		TWeakPtr<SWidget> LayoutSlateWidget = CurrentPrimaryLayout->GetCachedWidget();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogLab, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"), *GetName(), *GetNameSafe(InLocalPlayer), *GetNameSafe(CurrentPrimaryLayout));
			CurrentPrimaryLayout->RemoveFromParent();
			if (LayoutSlateWidget.IsValid())
			{
				UE_LOG(LogLab, Log, TEXT("root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist."), *GetNameSafe(InLocalPlayer));
			}
		}
	}
}

