#include "KLabGameplayTags.h"

#include "GameplayTagsManager.h"

FKLabGameplayTags FKLabGameplayTags::GameplayTags;

void FKLabGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);

	// Notify manager that we are done adding native tags.
	Manager.DoneAddingNativeTags();
}

void FKLabGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddAllUITags();
}

void FKLabGameplayTags::AddAllUITags()
{
	/* UI Layer */
	AddTag(UITag_Layer_Game, "UI.Layer.Game", "");
	AddTag(UITag_Layer_GameMenu, "UI.Layer.GameMenu", "");
	AddTag(UITag_Layer_Menu, "UI.Layer.Menu", "");
	AddTag(UITag_Layer_Debug, "UI.Layer.Debug", "");
	AddTag(UITag_Layer_Modal, "UI.Layer.Modal", "");
}

void FKLabGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

