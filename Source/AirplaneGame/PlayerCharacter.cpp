// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerGun.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FPSViewVec = FVector(28, 5, 70);
	TPSViewVec = FVector(-220, 5, 220);

	FPSViewRot = FRotator(0, 0, 0);
	TPSViewRot = FRotator(0, -35, 0);

	fpsCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamComp"));
	if (fpsCameraComp)
	{
		fpsCameraComp->SetupAttachment(RootComponent);
		fpsCameraComp->SetRelativeLocation(FPSViewVec);
		fpsCameraComp->SetRelativeRotation(FPSViewRot);
	}
}

bool APlayerCharacter::IsFalling()
{
	return isCanMove;
}

bool APlayerCharacter::IsGliding()
{
	return isMoveForwardnow;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	isSettingStarted = false;
	isCanMove = false;
	isMoveForwardnow = false;
	APPlayer = GetWorld()->GetFirstPlayerController();
	fallingSpeed = 1;
	ChangeGravity(1);

	FTransform firePosition = GetMesh()->GetSocketTransform(TEXT("GunPosition"));
	gun = GetWorld()->SpawnActor<APlayerGun>(GunFactory, firePosition);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	gun->AttachToComponent(GetMesh(), AttachRules);
	gun->SetActorLocation(firePosition.GetTranslation());
}

void APlayerCharacter::ChangeGravity(float Value)
{
	UCharacterMovementComponent* characterMovement = APPlayer->GetCharacter()->GetCharacterMovement();
	characterMovement->GravityScale = Value;
}

void APlayerCharacter::PlayerGlide(const FInputActionValue& Value)
{
	GetController()->GetPawn()->bUseControllerRotationPitch = false;
	GetController()->GetPawn()->SetActorRotation(FRotator::ZeroRotator);
	if (!isSettingStarted) return;
	isCanMove = true;
	isSettingStarted = false;
	ChangeGravity(0);
	fallingSpeed = 1;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	fpsCameraComp->SetRelativeLocation(TPSViewVec);
	fpsCameraComp->SetRelativeRotation(TPSViewRot);
}

void APlayerCharacter::PlayerPositionSetting(const FInputActionValue& Value)
{
	APawn* pawn = APPlayer->GetPawn();
	GetController()->GetPawn()->bUseControllerRotationPitch = true;
	if (isSettingStarted) return;
	isCanMove = false;
	isSettingStarted = true;
	ChangeGravity(1);
	FVector lauchVec = pawn->GetActorUpVector() * 2500;
	LaunchCharacter(lauchVec, true, true);

	fpsCameraComp->SetRelativeLocation(FPSViewVec);
	fpsCameraComp->SetRelativeRotation(FPSViewRot);
}

void APlayerCharacter::TurnPitch(const FInputActionValue& Value)
{
	float TurnAmount = -Value.Get<float>();
	AddControllerPitchInput(TurnAmount * 0.22f);
}

void APlayerCharacter::TurnYaw(const FInputActionValue& Value)
{
	float TurnAmount = Value.Get<float>();
	AddControllerYawInput(TurnAmount * 0.22f);
}

void APlayerCharacter::MoveForward(const FInputActionValue& Value)
{
	if (!isCanMove) return;
	float Movement = Value.Get<float>();
	isMoveForwardnow = true;
	FVector DeltaLocation = FVector(Movement * 4000 * UGameplayStatics::GetWorldDeltaSeconds(this), 0.0f, 0.0f);
	AddActorLocalOffset(DeltaLocation, true);
}

void APlayerCharacter::StopMoveForward()
{
	isMoveForwardnow = false;
	
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	if (!isCanMove) return;
	float Movement = Value.Get<float>();

	FVector DeltaLocation = FVector(0.0f, Movement * 1500 * UGameplayStatics::GetWorldDeltaSeconds(this), 0.0f);
	AddActorLocalOffset(DeltaLocation, true);
}

void APlayerCharacter::FireGun()
{
	if (isCanMove) return;

	if (gun)
	{
		gun->Fire();
		PlayAnimMontage(PlayerFireAnimMontage);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isSettingStarted)
	{
		FVector vec = GetCharacterMovement()->Velocity;
		UE_LOG(LogTemp, Log, TEXT("Velocity : %s"), *vec.ToString());
	}

	if (isCanMove)
	{
		FMath::Clamp(fallingSpeed += 9.8f + DeltaTime * 10, 0, 100);
		float Speed = fallingSpeed;
		if (isMoveForwardnow) Speed = fallingSpeed * 0.8f;

		FVector DeltaLocation = FVector(0, 0, -1 * Speed * DeltaTime);
		AddActorWorldOffset(DeltaLocation, true);
	}

}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(APMappingContext, 0);
		}
	}
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(PlayerSetUpAction, ETriggerEvent::Started, this,&APlayerCharacter::PlayerPositionSetting);
		EnhancedInputComponent->BindAction(GlideAction, ETriggerEvent::Started, this,&APlayerCharacter::PlayerGlide);
		EnhancedInputComponent->BindAction(TurnPitchAction, ETriggerEvent::Triggered, this,&APlayerCharacter::TurnPitch);
		EnhancedInputComponent->BindAction(TurnYawAction, ETriggerEvent::Triggered, this,&APlayerCharacter::TurnYaw);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this,&APlayerCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Completed, this,&APlayerCharacter::StopMoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this,&APlayerCharacter::MoveRight);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this,&APlayerCharacter::FireGun);
	}

}

