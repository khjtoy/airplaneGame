// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABullet.h"
#include "Animation/AnimInstance.h"
#include "PlayerGun.generated.h"

UCLASS()
class APlayerGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USkeletalMeshComponent* SkeletalMeshComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = Guns)
	TSubclassOf<class AABullet> bulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* GunFireAnimMontage;

	UAnimInstance* AnimInstance;	
};
