// Fill out your copyright notice in the Description page of Project Settings.
#include "FighterJet.h"
/* 추후 여기 경로 바꾸면 변경해야 함 */
#include "C:/Program Files/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "C:/Program Files/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include <Math/UnrealMathUtility.h>
#include "Kismet/KismetMathLibrary.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

AFighterJet::AFighterJet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈메시 데이터를 불려옴
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AirplaneContent/Mesh/SK_East_Fighter_Su30.SK_East_Fighter_Su30'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -120), FRotator(0, 0, 0));
	}

	// TPS카메라의 부모 클래스인 SpringArm 컴포넌트를 붙임.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	if (springArmComp)
	{
		springArmComp->SetupAttachment(RootComponent);
		springArmComp->SetRelativeLocation(FVector(0, 0, 130));
		springArmComp->TargetArmLength = 2500;
		springArmComp->SocketOffset = FVector(0, 0, 350);
	}

	// 카메라 컴포넌트를 붙이기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamComp"));
	if (tpsCamComp)
	{
		tpsCamComp->SetupAttachment(springArmComp);
	}


}

void AFighterJet::SetInput()
{
	SetupPlayerInputComponent(thisContorller->InputComponent);
}

void AFighterJet::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
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
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AFighterJet::MoveForward);
		// MoveRight
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AFighterJet::MoveRight);
		// SpaceBar
		EnhancedInputComponent->BindAction(SpacebarAction, ETriggerEvent::Triggered, this, &AFighterJet::PressSpaceBar);
		EnhancedInputComponent->BindAction(SpacebarAction, ETriggerEvent::Completed, this, &AFighterJet::StopSpaceBar);
		// LookUp
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AFighterJet::LookUp);
		// Turn
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AFighterJet::Turn);
		// Change Character view
		EnhancedInputComponent->BindAction(ChangePlayerAction, ETriggerEvent::Triggered, this, &AFighterJet::ChangePlayer);
	}
}

void AFighterJet::BeginPlay()
{
	Super::BeginPlay();

	// 이륙하기 전, 최소속도로 설정
	TargetThrustSpd = MinimumThrustSpd;
	thisContorller = GetController();
	if (thisContorller)
	{
		playerCharacter = GetWorld()->SpawnActor<ACharacter>(playerCharacterClass);
		playerCharacter->SetActorLocation(thisContorller->GetPawn()->GetActorLocation());
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
		playerCharacter->AttachToComponent(GetMesh(), AttachRules);
		playerCharacter->AddActorLocalOffset(FVector::UpVector * 500);

		playerCharacter->SetActorHiddenInGame(true);
		playerCharacter->SetActorEnableCollision(false);

		UE_LOG(LogTemp, Log, TEXT("Controller"));
	}
	else
		UE_LOG(LogTemp, Log, TEXT("Null"));
}

void AFighterJet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 땅에서 움직일 때는 낮은 값으로 설정된 Interp 값은 지상에 있을 때 점진적으로 속도가 빨라져야 함
	MoveForwardInterp = FMath::Clamp(FMath::FInterpTo(MoveForwardInterp, ForwardMovement, GetWorld()->DeltaTimeSeconds, 0.2f), 0.0f, 1.0f);

	if (Flying)
	{
		AddFlightMovement();
	}
}

void AFighterJet::MoveForward(const FInputActionValue& Value)
{
	ForwardMovement = Value.Get<float>();

	if (Flying)
	{
		TargetThrustSpd = FMath::Clamp((ForwardMovement * GetWorld()->DeltaTimeSeconds * 1500.0f) + TargetThrustSpd, MinimumThrustSpd, MaximumThrustSpd);
	}
	else
	{
		FVector forward = GetActorForwardVector() * MoveForwardInterp * (TargetThrustSpd / 500);
		forward.Z = 0.0f;
		AddActorWorldOffset(forward);

		if (MoveForwardInterp > 0.7f && keyDownSpaceBar)
		{
			Flying = true;
			springArmComp->bEnableCameraLag = true;
			springArmComp->bEnableCameraRotationLag = false;
			springArmComp->bUsePawnControlRotation = false;
			AddActorWorldOffset(FVector(0, 0, 0));
		}

		// Wheel Rotation
		FRotator SocketRot = GetMesh()->GetSocketRotation("front_wheel_jnt");

		SocketRot.Pitch += MoveForwardInterp * -500;
		SocketRot.Yaw = FMath::FInterpTo(WheelRotation.Yaw, UKismetMathLibrary::MapRangeClamped(RightMovement, -1.f, 1.f, WheelRotationSideMax * -1.f, WheelRotationSideMax), GetWorld()->DeltaTimeSeconds, 0.4f);

		WheelRotation = FRotator(SocketRot.Pitch, SocketRot.Yaw, 0.f);
	}
}

void AFighterJet::MoveRight(const FInputActionValue& Value)
{
	RightMovement = Value.Get<float>();

	float Movement = RightMovement * GetWorld()->DeltaTimeSeconds * 1.2f;

	if (Flying)
	{
		FlightRoll = FMath::FInterpTo(FlightRoll, RightMovement, GetWorld()->DeltaTimeSeconds, 10.f);

		AddActorLocalRotation(FRotator(0.f, 0.f, FlightRoll * GetWorld()->DeltaTimeSeconds * 90.0f));

		AileronFlapRotationLeft = FRotator(UKismetMathLibrary::MapRangeClamped(RightMovement, -1.f, 1.f, AileronInteriorRotationMax, AileronInteriorRotationMax * -1.0f), 0.f, 0.f);
		AileronFlapRotationRight = FRotator(UKismetMathLibrary::MapRangeClamped(RightMovement, -1.f, 1.f, AileronInteriorRotationMax * -1.0f, AileronInteriorRotationMax), 0.f, 0.f);
	}
	else
	{
		if (ForwardMovement > 0.f)
		{
			AddActorWorldRotation(FRotator(0.f, Movement, 0.f));
		}
	}
}

void AFighterJet::PressSpaceBar(const FInputActionValue& Value)
{
	keyDownSpaceBar = true;
}

void AFighterJet::StopSpaceBar(const FInputActionValue& Value)
{
	keyDownSpaceBar = false;
}

void AFighterJet::LookUp(const FInputActionValue& Value)
{
	float Amount = Value.Get<float>() * FMath::FloatSelect(-1.0f, 1.0f, InvertVerticalFlight);
	if (Flying)
	{
		FlightPitch = FMath::FInterpTo(FlightPitch, Amount, GetWorld()->DeltaTimeSeconds, 10.0f);

		FlightPitch = FlightPitch * GetWorld()->DeltaTimeSeconds * FlightTurnRate;

		AddActorLocalRotation(FRotator(Value.Get<float>(), 0.f, 0.f));

		AileronInteriorRotation = FRotator(UKismetMathLibrary::MapRangeClamped(Value.Get<float>(), -1.f, 1.f, AileronInteriorRotationMax, AileronInteriorRotationMax * -1.f), 0.f, 0.f);
	}
	else
	{
		AddControllerPitchInput(Amount);
	}
}

void AFighterJet::Turn(const FInputActionValue& Value)
{
	float Amount = Value.Get<float>();
	if (Flying)
	{
		FlightTurn = FMath::FInterpTo(FlightPitch, Amount, GetWorld()->DeltaTimeSeconds, 10.0f);

		FlightTurn = FlightTurn * GetWorld()->DeltaTimeSeconds * FlightTurnRate;

		AddActorLocalRotation(FRotator(0.f, 0.f, Value.Get<float>()));
		BackRudderRotation = FRotator(0.f, UKismetMathLibrary::MapRangeClamped(Value.Get<float>(), -1.f, 1.f, RudderRotationMax, RudderRotationMax * -1.f), 0.f);
	}
	else
	{
		AddControllerYawInput(Amount);
	}
}

void AFighterJet::ChangePlayer()
{
	if (playerCharacter)
	{
		thisContorller->UnPossess();
		thisContorller->Possess(playerCharacter);
		playerCharacter->SetupPlayerInputComponent(playerCharacter->InputComponent);

		APlayerCharacter* player = static_cast<APlayerCharacter*>(playerCharacter);
		player->SetActorHiddenInGame(false);
		player->SetActorEnableCollision(true);

		player->PlayerPositionSetting();
	}
}

void AFighterJet::AddFlightMovement()
{
	CurrentThrustSpd = FMath::FInterpTo(CurrentThrustSpd, TargetThrustSpd, GetWorld()->DeltaTimeSeconds, 8.0f);
	AddActorWorldOffset(GetMesh()->GetForwardVector() * (CurrentThrustSpd * GetWorld()->DeltaTimeSeconds), true);
}
