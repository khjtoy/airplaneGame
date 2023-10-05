// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"

UCLASS()
class AEnemyBullet : public AActor
{
	GENERATED_BODY()
public:
	AEnemyBullet();
public:	
	// �߻�ü�� �̵��� ����� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Movement)
		class UProjectileMovementComponent* movementComp;
	// �浹 ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class USphereComponent* collisionComp;
	// �ܰ� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
		class UStaticMeshComponent* bodyMeshComp;
public:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BulletMove();
};
