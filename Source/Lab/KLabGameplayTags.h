#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct  FKLabGameplayTags
{
public:
	static const FKLabGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	/**
	 *  UI GameplayTag
	 */
	FGameplayTag UITag_Layer_Game;
	FGameplayTag UITag_Layer_GameMenu;
	FGameplayTag UITag_Layer_Menu;
	FGameplayTag UITag_Layer_Modal;
	
	protected:
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddAllUITags();
	
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	
private:
	static FKLabGameplayTags GameplayTags;
};