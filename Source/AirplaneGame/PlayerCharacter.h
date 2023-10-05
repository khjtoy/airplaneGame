// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerGun.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	UFUNCTION(BlueprintCallable, Category = "Bool")
	bool IsFalling();
	UFUNCTION(BlueprintCallable, Category = "Bool")
	bool IsGliding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ChangeGravity(float Value);

	void PlayerGlide(const FInputActionValue& Value);
	void PlayerPositionSetting(const FInputActionValue& Value);
	void TurnPitch(const FInputActionValue& Value);
	void TurnYaw(const FInputActionValue& Value);
	void MoveForward(const FInputActionValue& Value);
	void StopMoveForward();
	void MoveRight(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void FireGun();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	APlayerController* APPlayer;

	bool isSettingStarted;
	bool isCanMove;
	bool isMoveForwardnow;

	FVector FPSViewVec;
	FVector TPSViewVec;
	FRotator FPSViewRot;
	FRotator TPSViewRot;

	FVector MoveVector;
	FVector ForwardVec;
	FVector RightVec;

	float fallingSpeed;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* APMappingContext;	

	UPROPERTY(VisibleAnyWhere, Category = Camera)
	class UCameraComponent* fpsCameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PlayerSetUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GlideAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TurnPitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TurnYawAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, Category = Guns)
	TSubclassOf<class APlayerGun> GunFactory;

	UPROPERTY(EditDefaultsOnly, Category = Guns)
	class UAnimMontage* PlayerFireAnimMontage;

	APlayerGun* gun;
};
