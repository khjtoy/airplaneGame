// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "AirplaneGame/PlayerCharacter.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyBullet::AEnemyBullet()
{
    // 1. 충돌체 등록하기
    collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    // 2. 충돌프로파일 설정
    collisionComp->SetCollisionProfileName(TEXT("BlockALL"));
    // 3. 충돌체 크기 설정
    collisionComp->SetSphereRadius(13);
    // 4. 루트로 등록
    RootComponent = collisionComp;
    // 5. 외관 컴포넌트 등록하기
    bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
    // 6. 부모 컴포넌트 지정하기
    bodyMeshComp->SetupAttachment(collisionComp);
    // 7. 충돌 비활성화
    bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // 8. 외관 크기 설정
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

