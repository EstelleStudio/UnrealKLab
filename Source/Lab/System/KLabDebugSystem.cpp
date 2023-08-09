// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabDebugSystem.h"

UKLabDebugSystem* UKLabDebugSystem::Instance = nullptr;

void UKLabDebugSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Instance = this;
	AddToRoot();
}

void UKLabDebugSystem::Deinitialize()
{
	Super::Deinitialize();
	Instance = nullptr;
	RemoveFromRoot();
}

void UKLabDebugSystem::Register(UObject* Obj)
{
#if KLAB_DEBUG
	if (!IsValid(Obj) || Obj->IsTemplate())
	{
		return;
	}

	const uint32 Id = Obj->GetUniqueID();
	if (!UObjectMap.Contains(Id))
	{
		UObjectMap.Add(Id, FKLabUObjectDebugData(Obj));	
		return;
	}
	check(false);
#endif
}

void UKLabDebugSystem::Unregister(UObject* Obj)
{
#if KLAB_DEBUG
	if (!IsValid(Obj) || Obj->IsTemplate())
	{
		return;
	}

	const uint32 Id = Obj->GetUniqueID();
	if (UObjectMap.Contains(Id))
	{
		UObjectMap.Remove(Id);
		return;
	}
	
	check(false);
#endif
}

void UKLabDebugSystem::AddString(UObject* Obj, const FString& StringData)
{
#if KLAB_DEBUG
	if (IsValid(Obj))
	{
		const uint32 Id = Obj->GetUniqueID();
		FKLabUObjectDebugData& DebugData = UObjectMap.FindChecked(Id);
		if (UObjectMap.Contains(Id))
		{
			DebugData.DebugStr.Append(StringData);
			return;
		}
	}
	check(false);
#endif
}

