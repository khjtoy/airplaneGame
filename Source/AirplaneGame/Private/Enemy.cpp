// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBullet.h"

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
	GetWorldTimerManager().SetTimer(fireTimer, this, &AEnemy::Fire, 5, true);
	GetWorldTimerManager().SetTimer(rotateTimer, this, &AEnemy::RotateAround, 1, true);
	GetWorldTimerManager().SetTimer(lookAtTimer, this, &AEnemy::LookAtPlayer, 0.01f, true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	//ToDO ::
	//플레이어의 포지션 값을 가져와서 그 방향으로 투사체 발사
	//비행기 주위로 회전하기
	//HP가 0이 될 시(아마 1방일수도 있음) 폭발 이펙트 나오면서 동시에 visible false
}

void AEnemy::RotateAround()
{
	//비행기 주위로 회전하기
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

