// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

UCLASS()
class AEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void Spawner();

public:
	UPROPERTY(EditDefaultsOnly, Category = variables)
		float distance = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = enemy)
		TSubclassOf<class AEnemy> enemy;
	FTimerHandle spawnTimer;
};
