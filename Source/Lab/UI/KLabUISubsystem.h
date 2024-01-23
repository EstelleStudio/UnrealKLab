// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KLabUISubsystem.generated.h"

class UKLabPrimaryLayout;
/**
 * 
 */
UCLASS()
class LAB_API UKLabUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual UWorld* GetWorld() const override;

protected:
	virtual void NotifyPlayerAdded(ULocalPlayer* InLocalPlayer);
    virtual void NotifyPlayerRemoved(ULocalPlayer* InLocalPlayer);
	
private:
	void CreatePrimaryLayoutWidget(ULocalPlayer* InLocalPlayer);
	void RemovePrimaryLayoutWidget(ULocalPlayer* InLocalPlayer);
	
	UPROPERTY(Transient)
	TObjectPtr<UKLabPrimaryLayout> CurrentPrimaryLayout = nullptr;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UKLabPrimaryLayout> LayoutClass;
};
