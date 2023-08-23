// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
/* 추후 여기 경로 바꾸면 변경해야 함 */
#include "C:\Program Files\UE_5.1\Engine\Plugins\EnhancedInput\Source\EnhancedInput\Public\InputAction.h"
#include "AirplaneCharacter.generated.h"

UCLASS()
class AIRPLANEGAME_API AAirplaneCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAirplaneCharacter();

protected:
	virtual void BeginPlay() override;

	/** * Callbacks for Input */
	void MoveForward(const FInputActionValue& Value);

# pragma region /* Input */
	/* MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* CombatMappingContext;

	/* MoveForward Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveForwardAction;
#pragma endregion

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
