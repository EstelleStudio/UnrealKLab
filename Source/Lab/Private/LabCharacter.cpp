// Fill out your copyright notice in the Description page of Project Settings.


#include "LabCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"


const float ALabCharacter::LookYawRate = 300.0f;
const float ALabCharacter::LookPitchRate = 165.0f;

static FName NAME_LabCharacterCollisionProfile_Capsule(TEXT("LabCharacterCapsule"));
static FName NAME_LabCharacterCollisionProfile_Mesh(TEXT("LabCharacterMesh"));

// Sets default values
ALabCharacter::ALabCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_LabCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); 
	MeshComp->SetCollisionProfileName(NAME_LabCharacterCollisionProfile_Mesh);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void ALabCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALabCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALabCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
			Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALabCharacter::Input_Move);
		}

		if (IA_LookMouse)
		{
			Input->BindAction(IA_LookMouse, ETriggerEvent::Triggered, this, &ALabCharacter::Input_LookMouse);
		}

		if (IA_LookStick)
		{
			Input->BindAction(IA_LookStick, ETriggerEvent::Triggered, this, &ALabCharacter::Input_LookStick);
		}
	}
}

void ALabCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	if (PlayerController)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, PlayerController->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ALabCharacter::Input_LookMouse(const FInputActionValue& InputActionValue)
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

void ALabCharacter::Input_LookStick(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X * LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y * LookPitchRate * World->GetDeltaSeconds());
	}
}

