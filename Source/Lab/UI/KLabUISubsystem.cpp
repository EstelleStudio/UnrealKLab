// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabUISubsystem.h"

void UKLabUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
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

