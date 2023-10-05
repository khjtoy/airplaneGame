// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerGun.h"
#include "ABullet.h"

// Sets default values
APlayerGun::APlayerGun()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerGun::BeginPlay()
{
	Super::BeginPlay();
	SkeletalMeshComponent = FindComponentByClass<USkeletalMeshComponent>();
	if(SkeletalMeshComponent) AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	CoolTime = 0;
}

// Called every frame
void APlayerGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CoolTime > 0)
	{
		CoolTime -= DeltaTime;
	}
}

void APlayerGun::Fire()
{
	if (CoolTime <= 0)
	{
		if (SkeletalMeshComponent && GunFireAnimMontage)
		{
			FTransform firePosition = SkeletalMeshComponent->GetSocketTransform(TEXT("FirePosition"));
			AABullet* bullet = GetWorld()->SpawnActor<AABullet>(bulletFactory, firePosition);
			bullet->SetActorRotation(SkeletalMeshComponent->GetComponentRotation());
			AnimInstance->Montage_Play(GunFireAnimMontage);
		}
		CoolTime = 3;
	}
}

