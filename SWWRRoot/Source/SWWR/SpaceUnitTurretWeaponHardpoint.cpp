// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "SpaceUnitTurretWeaponHardpoint.h"




ASpaceUnitTurretWeaponHardpoint::ASpaceUnitTurretWeaponHardpoint() {

	TurretBase = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Turretbase"));
	TurretBase->SetupAttachment(RootComponent);

	TurretTop = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("TurretTop"));
	TurretTop->SetupAttachment(TurretBase);
}

void ASpaceUnitTurretWeaponHardpoint::BeginPlay()
{

}

void ASpaceUnitTurretWeaponHardpoint::CustomTick(float DeltaSeconds)
{
	Super::CustomTick(DeltaSeconds);
	if (bIsAttacking)
	{
		FVector Dir = (TargetLoc - GetActorLocation());
		Dir.Normalize();
		FQuat TweenVec = FQuat::FindBetweenVectors(TurretBase->GetForwardVector(), Dir);
		FVector Dir2 = (TargetLoc - (GetActorLocation() + TurretTop->RelativeLocation));
		Dir2.Normalize();
		FQuat TweenVec2 = FQuat::FindBetweenVectors(TurretTop->GetForwardVector(), Dir2);
		float Bearing = TweenVec.Rotator().Yaw;
		float Bearing2 = TweenVec2.Rotator().Yaw;
		float Elev = TweenVec.Rotator().Pitch;
		float Elev2 = TweenVec2.Rotator().Pitch;
		TurretBase->SetRelativeRotation(FRotator(0, TurretBase->RelativeRotation.Yaw + FMath::Clamp(Bearing, -5.f, 5.f), 0));

		TurretTop->SetRelativeRotation(FRotator(Elev, 0, 0));// FRotator(Elev, Bearing, 0));
		//TurretTop->SetWorldRotation(TurretTop->GetComponentRotation() + FRotator(FMath::Clamp(Elev2, -0.5f, 0.5f), 0, 0));
	}
	//TurretBase->SetRelativeRotation(FRotator(0.f, FMath::Sin(GetWorld()->TimeSeconds / 10.f)*360.f, 0.f));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Child Actor Tick"));
}