// Fill out your copyright notice in the Description page of Project Settings.


#include "KLabALSCharacter.h"

#include "AlsCharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/KLabALSComponent.h"
#include "Utility/AlsMath.h"


AKLabALSCharacter::AKLabALSCharacter(const FObjectInitializer& ObjectInitializer) : Super{
	ObjectInitializer.SetDefaultSubobjectClass<UAlsCharacterMovementComponent>(CharacterMovementComponentName)
}
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = false;
	AlsCharacterMovement = Cast<UAlsCharacterMovementComponent>(GetCharacterMovement());
	ALSComponent = CreateDefaultSubobject<UKLabALSComponent>(TEXT("ALS Component"));
}

void AKLabALSCharacter::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{	
	ALSComponent->OnMovementModeChanged(GetCharacterMovement()->MovementMode);
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

FRotator AKLabALSCharacter::GetViewRotation() const
{
	return ALSComponent->GetViewRotation();
}

// Called when the game starts or when spawned
void AKLabALSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKLabALSCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	if (PlayerController)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		// const FRotator MovementRotation(0.0f, PlayerController->GetControlRotation().Yaw, 0.0f);
		//
		const auto ForwardDirection{UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(ALSComponent->GetViewState().Rotation.Yaw))};
		const auto RightDirection{UAlsMath::PerpendicularCounterClockwiseXY(ForwardDirection)};

		AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);

	}
}

void AKLabALSCharacter::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y);
	}
}

// Called every frame
void AKLabALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKLabALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer());
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 100);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input)
	{
		if (IA_Move)
		{
			Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKLabALSCharacter::Input_Move);
		}
		if (IA_LookMouse)
		{
			Input->BindAction(IA_LookMouse, ETriggerEvent::Triggered, this, &AKLabALSCharacter::Input_LookMouse);
		}
	}
}

