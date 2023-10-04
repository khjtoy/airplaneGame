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
	//5�� �������� Fire ����
	GetWorldTimerManager().SetTimer(fireTimer, this, &AEnemy::Fire, 5, true);
	GetWorldTimerManager().SetTimer(rotateTimer, this, &AEnemy::RotateAround, 1, true);
	GetWorldTimerManager().SetTimer(lookAtTimer, this, &AEnemy::LookAtPlayer, 0.01f, true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	//ToDO ::
	//�÷��̾��� ������ ���� �����ͼ� �� �������� ����ü �߻�
	//����� ������ ȸ���ϱ�
	//HP�� 0�� �� ��(�Ƹ� 1���ϼ��� ����) ���� ����Ʈ �����鼭 ���ÿ� visible false
}

void AEnemy::RotateAround()
{
	//����� ������ ȸ���ϱ�
}

void AEnemy::LookAtPlayer()
{
	// �÷��̾� ���� �ٶ󺸱�
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FRotator dir = UKismetMathLibrary::FindLookAtRotation(playerPos, GetActorLocation());

	SetActorRotation(dir);
}

void AEnemy::Fire()
{
	//����ü �߻�
	GetWorld()->SpawnActor<AEnemyBullet>(bulletFactory, GetActorLocation() + firePos, FRotator(GetActorRotation().Pitch + 180.f, GetActorRotation().Yaw, GetActorRotation().Roll + 180.f));
}

