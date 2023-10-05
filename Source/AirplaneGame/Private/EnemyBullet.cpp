// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "AirplaneGame/PlayerCharacter.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyBullet::AEnemyBullet()
{
    // 1. �浹ü ����ϱ�
    collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    // 2. �浹�������� ����
    collisionComp->SetCollisionProfileName(TEXT("BlockALL"));
    // 3. �浹ü ũ�� ����
    collisionComp->SetSphereRadius(13);
    // 4. ��Ʈ�� ���
    RootComponent = collisionComp;
    // 5. �ܰ� ������Ʈ ����ϱ�
    bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
    // 6. �θ� ������Ʈ �����ϱ�
    bodyMeshComp->SetupAttachment(collisionComp);
    // 7. �浹 ��Ȱ��ȭ
    bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // 8. �ܰ� ũ�� ����
    bodyMeshComp->SetRelativeScale3D(FVector(0.25));

    movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    movementComp->SetUpdatedComponent(collisionComp);
    movementComp->InitialSpeed = 6000.0f;
    movementComp->MaxSpeed = 6000.0f;
    movementComp->bRotationFollowsVelocity = true;
    movementComp->bShouldBounce = true;
    movementComp->Bounciness = 0.3f;
    movementComp->ProjectileGravityScale = 0.0f;

}

void AEnemyBullet::Tick(float DeltaTime)
{
    BulletMove();
}

void AEnemyBullet::BulletMove()
{
    movementComp->Velocity = GetActorForwardVector();
}

void AEnemyBullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    if (Other->IsA<APlayerCharacter>())
    {
        UE_LOG(LogTemp, Log, TEXT("HI"));
        //Cast<AAirplaneCharacter>(Other)->
        Destroy();
    }
}

