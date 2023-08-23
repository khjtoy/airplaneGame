// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AirplaneCharacter.h"
/* 추후 여기 경로 바꾸면 변경해야 함 */
#include "C:/Program Files/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "C:/Program Files/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

AAirplaneCharacter::AAirplaneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAirplaneCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAirplaneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	APlayerController* AirplaneController = Cast<APlayerController>(Controller);

	if (AirplaneController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(AirplaneController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(CombatMappingContext, 0);
		}
	}

	// Set up action bindings
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		// MoveForward
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AAirplaneCharacter::MoveForward);
	}
}

void AAirplaneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAirplaneCharacter::MoveForward(const FInputActionValue& Value)
{
	float Movement = Value.Get<float>();
	if (Controller && (Movement != 0.f))
	{
		// find out which way is forward
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Movement);
	}
}

