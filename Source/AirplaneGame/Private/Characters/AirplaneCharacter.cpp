// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AirplaneCharacter.h"
/* ���� ���� ��� �ٲٸ� �����ؾ� �� */
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

AAirplaneCharacter::AAirplaneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// TPSī�޶��� �θ� Ŭ������ SpringArm ������Ʈ ���̱�
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	if (springArmComp)
	{
		springArmComp->SetupAttachment(RootComponent);
		springArmComp->SetRelativeLocation(FVector(0, 70, 90));
		springArmComp->TargetArmLength = 400;
	}
	// ī�޶� ������Ʈ ���̱�
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamComp"));
	if (tpsCamComp)
	{
		tpsCamComp->SetupAttachment(springArmComp);
	}
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
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AAirplaneCharacter::MoveForward);
		EnhancedInputComponent->BindAction(TurnPitchAction, ETriggerEvent::Triggered, this, &AAirplaneCharacter::TurnPitch);
	}
}

void AAirplaneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAirplaneCharacter::MoveForward(const FInputActionValue& Value)
{
	float Movement = Value.Get<float>();
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// add movement
		AddMovementInput(ForwardDirection, Movement);
	}
}

void AAirplaneCharacter::TurnPitch(const FInputActionValue& Value)
{
	float Turn = Value.Get<float>();
	FRotator CurrentRotation = GetMesh()->GetRelativeRotation();

	float pitchValue = FMath::Clamp(CurrentRotation.Pitch + (Turn * turnSpeed), minPitch, maxPitch);

	FRotator NewRotation = FRotator(pitchValue, CurrentRotation.Yaw, CurrentRotation.Roll);
	GetMesh()->SetRelativeRotation(NewRotation);
}

