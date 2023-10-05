// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerGun.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/SphereComponent.h>
#include "FighterJet.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FPSViewVec = FVector(28, 5, 70);

	FPSViewRot = FRotator(0, 0, 0);

	fpsCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamComp"));
	if (fpsCameraComp)
	{
		fpsCameraComp->SetupAttachment(RootComponent);
		fpsCameraComp->SetRelativeLocation(FPSViewVec);
		fpsCameraComp->SetRelativeRotation(FPSViewRot);
	}

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlockALL"));
	collisionComp->SetSphereRadius(30);
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
	fallingRemainingTime = 0;

	APPlayer = UGameplayStatics::GetPlayerController(this, 0);

	UE_LOG(LogTemp, Log, TEXT("Controller :: %s"), *UGameplayStatics::GetPlayerController(this, 0)->GetName());

	fallingSpeed = 1;

	FTransform firePosition = GetMesh()->GetSocketTransform(TEXT("GunPosition"));
	gun = GetWorld()->SpawnActor<APlayerGun>(GunFactory, firePosition);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	gun->AttachToComponent(GetMesh(), AttachRules);
	gun->SetActorLocation(firePosition.GetTranslation());

}

void APlayerCharacter::ChangeGravity(float Value)
{
	UCharacterMovementComponent* characterMovement = GetController()->GetCharacter()->GetCharacterMovement();
	characterMovement->GravityScale = Value;
}

void APlayerCharacter::PlayerGlide()
{
	if (!isSettingStarted) return;
	fallingRemainingTime = 0;
	isCanMove = true;
	isSettingStarted = false;
	ChangeGravity(0);
	fallingSpeed = 1;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void APlayerCharacter::PlayerPositionSetting()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->EnableInput(PlayerController);
	//UE_LOG(LogTemp, Log, TEXT("Controller : %s"), *UGameplayStatics::GetPlayerController(this, 0)->GetName());

	thisContorller = GetController();
	APawn* pawn = thisContorller->GetPawn();

	if (isSettingStarted) return;
	isCanMove = false;
	fallingTime = 0;
	isSettingStarted = true;

	ChangeGravity(1);
	FVector lauchVec = pawn->GetActorUpVector() * 4000;
	LaunchCharacter(lauchVec, true, true);

	fpsCameraComp->SetRelativeLocation(FPSViewVec);
	fpsCameraComp->SetRelativeRotation(FPSViewRot);

	gun->SetActorHiddenInGame(false);
	gun->SetActorEnableCollision(true);
}

void APlayerCharacter::Die()
{
	UE_LOG(LogTemp, Log, TEXT("Die"));
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
	FVector DeltaLocation = FVector(Movement * 500 * UGameplayStatics::GetWorldDeltaSeconds(this), 0.0f, 0.0f);
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

	FVector DeltaLocation = FVector(0.0f, Movement * 450 * UGameplayStatics::GetWorldDeltaSeconds(this), 0.0f);
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
		fallingTime += DeltaTime;
		if (fallingTime >= 4)
		{
			PlayerGlide();
		}
	}

	if (isCanMove)
	{
		FMath::Clamp(fallingSpeed += 9.8f + DeltaTime * 100, 0, 100);
		float Speed = fallingSpeed;
		if (isMoveForwardnow) Speed = fallingSpeed * 0.9f;

		FVector DeltaLocation = FVector(0, 0, -1 * Speed * DeltaTime);
		AddActorWorldOffset(DeltaLocation, true);

		fallingRemainingTime += DeltaTime;
		if (fallingRemainingTime > 5)
		{
			Die();
			fallingRemainingTime = 0;
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
		EnhancedInputComponent->BindAction(PlayerSetUpAction, ETriggerEvent::Started, this, &APlayerCharacter::PlayerPositionSetting);
		EnhancedInputComponent->BindAction(GlideAction, ETriggerEvent::Started, this, &APlayerCharacter::PlayerGlide);
		EnhancedInputComponent->BindAction(TurnPitchAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnPitch);
		EnhancedInputComponent->BindAction(TurnYawAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnYaw);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopMoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::FireGun);
	}
}

void APlayerCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!isCanMove) return;
	isCanMove = false;
	if (Other->IsA<AFighterJet>())
	{
		thisContorller->GetPawn()->SetActorRotation(FRotator::ZeroRotator);

		thisContorller->UnPossess();
		thisContorller->Possess(Cast<ACharacter>(Other));

		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);

		gun->SetActorHiddenInGame(true);
		gun->SetActorEnableCollision(false);

		Cast<ACharacter>(Other)->SetupPlayerInputComponent(Cast<ACharacter>(Other)->InputComponent);
	}
}
