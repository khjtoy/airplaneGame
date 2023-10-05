// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AirplaneGame/PlayerCharacter.h"
/* ���� ���� ��� �ٲٸ� �����ؾ� �� */
#include "C:\Program Files\UE_5.1\Engine\Plugins\EnhancedInput\Source\EnhancedInput\Public\InputAction.h"
#include "FighterJet.generated.h"

UCLASS()
class AIRPLANEGAME_API AFighterJet : public ACharacter
{
	GENERATED_BODY()

public:
	AFighterJet();
	void SetInput();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* tpsCamComp;

	UPROPERTY(EditAnywhere, Category = Speed)
	float MinimumThrustSpd = 5000.f;

	UPROPERTY(EditAnywhere, Category = Speed)
	float MaximumThrustSpd = 25000.f;

	UPROPERTY(EditAnywhere, Category = Rotation)
	float FlightTurnRate = 45.f;

	UPROPERTY(EditAnywhere, Category = MeshTransforms)
	float WheelRotationSideMax = 15.f;

	UPROPERTY(EditAnywhere, Category = MeshTransforms)
	float  AileronInteriorRotationMax = 30.f;

	UPROPERTY(EditAnywhere, Category = MeshTransforms)
	float RudderRotationMax = 25.f;

	UPROPERTY(EditAnywhere, Category = PlayerClass)
	TSubclassOf<APlayerCharacter> playerCharacterClass;

	AController* thisContorller;

	ACharacter* playerCharacter;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Speed)
	float TargetThrustSpd;
	UPROPERTY(BlueprintReadOnly, Category = Speed)
	float CurrentThrustSpd;
	UPROPERTY(BlueprintReadOnly)
	float MoveForwardInterp;
	UPROPERTY(BlueprintReadOnly)
	float ForwardMovement;
	UPROPERTY(BlueprintReadOnly)
	float RightMovement;
	UPROPERTY(BlueprintReadOnly)
	bool Flying = false;
	UPROPERTY(BlueprintReadOnly)
	bool keyDownSpaceBar = false;
	UPROPERTY(BlueprintReadOnly)
	bool InvertVerticalFlight = true;
	UPROPERTY(BlueprintReadOnly, Category = Rotation)
	float FlightPitch;
	UPROPERTY(BlueprintReadOnly, Category = Rotation)
	float FlightTurn;
	UPROPERTY(BlueprintReadOnly, Category = MeshTransforms)
	FRotator WheelRotation;
	UPROPERTY(BlueprintReadOnly, Category = MeshTransforms)
	FRotator AileronInteriorRotation;
	UPROPERTY(BlueprintReadOnly, Category = MeshTransforms)
	FRotator AileronFlapRotationLeft;
	UPROPERTY(BlueprintReadOnly, Category = MeshTransforms)
	FRotator AileronFlapRotationRight;
	UPROPERTY(BlueprintReadOnly, Category = MeshTransforms)
	FRotator BackRudderRotation;
	UPROPERTY(BlueprintReadOnly, Category = PlaneRotation)
	float FlightRoll;

	/** * Callbacks for Input */
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void PressSpaceBar(const FInputActionValue& Value);
	void StopSpaceBar(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void ChangePlayer();

# pragma region /* Input */
	/* MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* CombatMappingContext;

	/* MoveForward Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveForwardAction;

	/* MoveRight Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveRightAction;

	/* Spacebar Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SpacebarAction;

	/* LookUp Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookUpAction;

	/* Turn Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* TurnAction;

	/* PlayerJump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ChangePlayerAction;
#pragma endregion
private:
	void AddFlightMovement();
};
