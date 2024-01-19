// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabWidgetBase.h"

TSharedPtr<FSlateUser> UKLabWidgetBase::GetOwnerSlateUser() const
{
	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	return LocalPlayer ? LocalPlayer->GetSlateUser() : nullptr;
}
