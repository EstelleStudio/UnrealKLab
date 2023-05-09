// Fill out your copyright notice in the Description page of Project Settings.


#include "LabMovableSceneCapMirror.h"

// Sets default values
ALabMovableSceneCapMirror::ALabMovableSceneCapMirror()
{
	PrimaryActorTick.bCanEverTick = false;
	MirrorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mirror"));
	RootComponent = MirrorMeshComp;
}
