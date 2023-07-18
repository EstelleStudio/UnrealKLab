// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabSlider.generated.h"

UCLASS()
class ALabSlider : public AActor
{
	GENERATED_BODY()

public:
	ALabSlider();

protected:
	virtual void BeginPlay() override;

	// Rep Function
	UFUNCTION()
	void OnRep_CurrentValue();

public:
	virtual void Tick(float DeltaTime) override;

protected:
	// Mesh Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KLab")
	TObjectPtr<UStaticMeshComponent> SliderBaseMeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KLab")
	TObjectPtr<UStaticMeshComponent> SliderLeftCapMeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KLab")
	TObjectPtr<UStaticMeshComponent> SliderRightCapMeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KLab")
	TObjectPtr<UStaticMeshComponent> SliderMeshComp;

	// Boolean Property
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KLab")
	bool bDrawText = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KLab")
	bool bFlipText = false;

	// Value Property
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="0.0", ClampMax="1.0"), Category = "KLab")
	float DefaultValue = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KLab")
	FVector2D Range = FVector2D(0.f, 1.f);

	// Rep Property
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentValue, Category = "KLab")
	float CurrentValue = 0.0f;

public :
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void InitSliderPosition();
	void InitSliderBaseRotationAndScale();
	void AddTextMarkers();
};
