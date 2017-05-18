// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "ProjectileBase.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + (GetActorForwardVector()*ProjectileSpeed*DeltaTime), DeltaTime*100.f));
}

