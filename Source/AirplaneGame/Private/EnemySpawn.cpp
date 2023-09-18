// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	GetWorldTimerManager().SetTimer(spawnTimer, this, &AEnemySpawn::Spawner, 3, true);
}

void AEnemySpawn::Spawner()
{
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	
	const float pitch = FMath::FRandRange(-1.f, 1.f);
	const float yaw = FMath::FRandRange(-1.f, 1.f);
	const float roll = FMath::FRandRange(-1.f, 1.f);

	FVector randomPos = FVector(pitch, yaw, roll) * distance;
	//randomPos = randomPos.GetSafeNormal(distance);

	FVector realSpawnPos = playerPos + randomPos;
	FRotator rotator;
	FActorSpawnParameters spawnParam;

	GetWorld()->SpawnActor<AActor>(enemy->GeneratedClass, realSpawnPos, rotator, spawnParam);
}

