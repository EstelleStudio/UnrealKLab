// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabPrimaryDataAsset.h"

#include "GameFeatureAction.h"

#define LOCTEXT_NAMESPACE "KLabLoc"

UKLabPrimaryDataAsset::UKLabPrimaryDataAsset()
{
}

#if WITH_EDITOR
EDataValidationResult UKLabPrimaryDataAsset::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	// 1. Make sure didn't subclass from a BP of this (it's fine and expected to subclass once in BP, just not twice)
	if (!GetClass()->IsNative())
	{
		UClass* ParentClass = GetClass()->GetSuperClass();

		// Find the native parent
		UClass* FirstNativeParent = ParentClass;
		while ((FirstNativeParent != nullptr) && !FirstNativeParent->IsNative())
		{
			FirstNativeParent = FirstNativeParent->GetSuperClass();
		}

		if (FirstNativeParent != ParentClass)
		{
			ValidationErrors.Add(FText::Format(LOCTEXT("KLab InheritenceIsUnsupported","Parent class was {0} but should be {1}."), 
				FText::AsCultureInvariant(GetPathNameSafe(ParentClass)),
				FText::AsCultureInvariant(GetPathNameSafe(FirstNativeParent))
			));
			Result = EDataValidationResult::Invalid;
		}
	}

	// 2. Check All Actions
	int32 EntryIndex = 0;
	for (UGameFeatureAction* Action : GameFeatureActions)
	{
		if (Action)
		{
			EDataValidationResult ChildResult = Action->IsDataValid(ValidationErrors);
			Result = CombineDataValidationResults(Result, ChildResult);
		}
		else
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::Format(LOCTEXT("ActionEntryIsNull", "Null entry at index {0} in Actions"), FText::AsNumber(EntryIndex)));
		}

		++EntryIndex;
	}
	
	return Result;
}
#endif

#if WITH_EDITORONLY_DATA
void UKLabPrimaryDataAsset::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();
	for (UGameFeatureAction* Action : GameFeatureActions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif
