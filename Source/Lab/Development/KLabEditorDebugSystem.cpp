// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabEditorDebugSystem.h"

UKLabEditorDebugSystem* UKLabEditorDebugSystem::Instance = nullptr;

void UKLabEditorDebugSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Instance = this;
	AddToRoot();
}

void UKLabEditorDebugSystem::Deinitialize()
{
	Super::Deinitialize();
	Instance = nullptr;
	RemoveFromRoot();
}

void UKLabEditorDebugSystem::Register(UObject* Obj)
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

void UKLabEditorDebugSystem::Unregister(UObject* Obj)
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

void UKLabEditorDebugSystem::AddString(UObject* Obj, const FString& StringData)
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

