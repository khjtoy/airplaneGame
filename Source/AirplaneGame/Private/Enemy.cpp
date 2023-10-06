// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBullet.h"
#include "Particles/ParticleSystem.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	//5초 간격으로 Fire 실행
	GetWorldTimerManager().SetTimer(fireTimer, this, &AEnemy::Fire, 3, true);
	GetWorldTimerManager().SetTimer(moveTimer, this, &AEnemy::MoveToPlayer, 0.01f, true);
	GetWorldTimerManager().SetTimer(lookAtTimer, this, &AEnemy::LookAtPlayer, 0.01f, true);
}

void AEnemy::MoveToPlayer()
{
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), playerPos, GetWorld()->GetDeltaSeconds(), 0.3f));
}

void AEnemy::LookAtPlayer()
{
	// 플레이어 방향 바라보기
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FRotator dir = UKismetMathLibrary::FindLookAtRotation(playerPos, GetActorLocation());

	SetActorRotation(dir);
}

void AEnemy::Fire()
{
	//투사체 발사
	GetWorld()->SpawnActor<AEnemyBullet>(bulletFactory, GetActorLocation() + firePos, FRotator(GetActorRotation().Pitch + 180.f, GetActorRotation().Yaw, GetActorRotation().Roll + 180.f));
}

void AEnemy::GetHit()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), deathExp, GetActorLocation(), FRotator(0, 0, 0));
	this->SetActorHiddenInGame(true);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

