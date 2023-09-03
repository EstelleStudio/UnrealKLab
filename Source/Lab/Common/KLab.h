#pragma once

#include "Logging/LogMacros.h"
#include "NativeGameplayTags.h"

namespace KLabGameplayTags
{
	LAB_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned)
	
}

LAB_API DECLARE_LOG_CATEGORY_EXTERN(LogLab, Log, All)

#define KLAB_EDITOR_DEBUG WITH_EDITOR && ENABLE_DRAW_DEBUG