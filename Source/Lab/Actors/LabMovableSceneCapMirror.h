// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabMovableSceneCapMirror.generated.h"

UCLASS()
class ALabMovableSceneCapMirror : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALabMovableSceneCapMirror();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KLab|Material", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UStaticMeshComponent> MirrorMeshComp;

};
