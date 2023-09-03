﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Common/KLab.h"
#include "KLabEditorDebugSystem.generated.h"

USTRUCT()
struct FKLabUObjectDebugData
{
	GENERATED_BODY()
	FKLabUObjectDebugData () = default;	
	explicit FKLabUObjectDebugData(UObject* InObj) : ObjectPtr(InObj)
	{
		DebugStr = FString::Printf(TEXT("Register frame: %lld\n"), GFrameCounter);
	}

	FWeakObjectPtr ObjectPtr = nullptr;

	UPROPERTY(VisibleAnywhere, Transient)
	FString DebugStr;
};

#if KLAB_EDITOR_DEBUG

#define KLAB_DEBUG_REGISTER() UKLabEditorDebugSystem::GetInstance()->Register(this);
#define KLAB_DEBUG_UNREGISTER() UKLabEditorDebugSystem::GetInstance()->Unregister(this);
#define KLAB_DEBUG_ADDSTR(STR) UKLabEditorDebugSystem::GetInstance()->AddString(this, STR);

#else

#define KLAB_DEBUG_REGISTER()
#define KLAB_DEBUG_ADDSTR(STR)

#endif

UCLASS()
class LAB_API UKLabEditorDebugSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void Register(UObject* Obj);
	void Unregister(UObject* Obj);
	void AddString(UObject* Obj, const FString& StringData);

	static UKLabEditorDebugSystem* GetInstance() { return Instance; }

private:
	static UKLabEditorDebugSystem* Instance;
	
	UPROPERTY(VisibleAnywhere, Transient)
	TMap<uint32, FKLabUObjectDebugData> UObjectMap;
};
