// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"
#include "Enemy.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	GetWorldTimerManager().SetTimer(spawnTimer, this, &AEnemySpawn::Spawner, 10, true);
}

void AEnemySpawn::Spawner()
{
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	
	for (int i = 0; i < 3; i++)
	{
		const float pitch = FMath::FRandRange(-1.f, 1.f);
		const float yaw = FMath::FRandRange(-1.f, 1.f);

		FVector randomPos = FVector(pitch, yaw, 0.f) * distance;

		FVector realSpawnPos = playerPos + randomPos;
		FRotator rotator;
		FActorSpawnParameters spawnParam;

		GetWorld()->SpawnActor<AEnemy>(enemy, realSpawnPos, rotator, spawnParam);
	}
}

