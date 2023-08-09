// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Common/KLab.h"
#include "KLabDebugSystem.generated.h"

#if KLAB_DEBUG

#define KLAB_DEBUG_REGISTER() UKLabDebugSystem::GetInstance()->Register(this);
#define KLAB_DEBUG_ADDSTR(STR) UKLabDebugSystem::GetInstance()->AddString(this, STR);

#else

#define KLAB_DEBUG_REGISTER()
#define KLAB_DEBUG_ADDSTR(STR)

#endif

UCLASS()
class LAB_API UKLabDebugSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void Register(UObject* Obj);
	void Unregister(UObject* Obj);
	void AddString(UObject* Obj, const FString& StringData);

	static UKLabDebugSystem* GetInstance() { return Instance; }

private:
	static UKLabDebugSystem* Instance;
	
#if KLAB_DEBUG
	struct FKLabUObjectDebugData
	{
		explicit FKLabUObjectDebugData(UObject* InObj) : ObjectPtr(InObj)
		{
			DebugStr = FString::Printf(TEXT("Register frame: %lld\n"), GFrameCounter);
		}

		FWeakObjectPtr ObjectPtr = nullptr;
		FString DebugStr;
	};

	TMap<uint32, FKLabUObjectDebugData> UObjectMap;
#endif
};
