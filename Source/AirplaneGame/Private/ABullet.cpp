// Fill out your copyright notice in the Description page of Project Settings.


#include "ABullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
AABullet::AABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 35.0f;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlockALL"));
	collisionComp->SetSphereRadius(13);
	RootComponent = collisionComp;
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->ProjectileGravityScale = 0.01f;
	movementComp->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABullet::Die()
{
}

