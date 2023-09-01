// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class AIRPLANEGAME_API AEnemy : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AEnemyBullet> bulletFactory;
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void RotateAround();
	virtual void LookAtPlayer();
	virtual void Fire();

public:
	FTimerHandle fireTimer;
};
