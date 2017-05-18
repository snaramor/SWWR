// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "BaseMobileSpaceUnit.h"

ABaseMobileSpaceUnit::ABaseMobileSpaceUnit()
{
	//Constructor
}
void ABaseMobileSpaceUnit::BeginPlay() {
	Super::BeginPlay();
	MoveTarget = GetActorLocation();
}


// Called every frame
void ABaseMobileSpaceUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SelectionCircle->SetWorldRotation(FRotator::ZeroRotator);


	if (TargetPos != FVector::ZeroVector)
	{

	}


	if (MoveType != EMoveTypeEnum::Finished)
	{

		float UnitRadius = this->RootComponent->Bounds.SphereRadius;
		FVector Dir = (MoveTarget - GetActorLocation());
		Dir.Normalize();
		FVector Forward = GetActorForwardVector();
		Forward.Normalize();
		float Dist = FVector::Dist(GetActorLocation(), MoveTarget);
		float Dot = FVector::DotProduct(Dir, Forward);
		float Bearing = FQuat::FindBetweenVectors(Forward, Dir).Rotator().Yaw;
		float Roll = FMath::Clamp(Bearing / 4, -MaxBank, MaxBank);
		FVector TargMov = GetActorLocation() + (GetActorForwardVector()*MaxSpeed);
		FRotator TargetRot = FRotator(0, GetActorRotation().Yaw + FMath::Clamp(Bearing, -TurnSpeed, TurnSpeed), Roll);
		switch (MoveType)
		{
		case EMoveTypeEnum::TurnOnly:
			Roll = 0;
			this->SetActorRotation(FMath::Lerp(this->GetActorRotation(), TargetRot, DeltaTime));
			break;
		case EMoveTypeEnum::TurnThanGo:

			this->SetActorRotation(FMath::Lerp(this->GetActorRotation(), TargetRot, DeltaTime));
			if (FMath::Abs(Bearing) < 3.0f)
			{
				//SetActorLocation(GetActorLocation() + GetActorForwardVector()*DeltaTime*MaxSpeed);
				SetActorLocation(FMath::Lerp(GetActorLocation(), TargMov, DeltaTime));
			}
			break;
		case EMoveTypeEnum::TurnAndGo:
			SetActorLocation(FMath::Lerp(GetActorLocation(), TargMov, DeltaTime));
			this->SetActorRotation(FMath::Lerp(this->GetActorRotation(), TargetRot, DeltaTime));
			break;
		default:
			break;
		}
		if (Dist < (UnitRadius*0.2f))
		{
			MoveType = EMoveTypeEnum::Finished;
		}

		//float Roll = FMath::Clamp(Bearing / 3, -45.f, 45.f);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dot: ") + FString::SanitizeFloat(Dot) + TEXT(" || Bearing: ") + FString::SanitizeFloat(Bearing) + TEXT(" || TargetRot: ") + TargetRot.ToString());
		//if (Dist < this->RootComponent->Bounds.SphereRadius)//FMath::Abs(Bearing) > 10.f)//Dist < this->RootComponent->Bounds.SphereRadius
			//TargetRot = TargetRot + FRotator(0.f, FMath::Acos(Bearing) * 45.f, 0.f);

			//FRotator TargetRot = FRotationMatrix::MakeFromX(Dir).Rotator();
			//this->SetActorRotation(GetActorRotation() + FRotator(1 - Bearing * 15.f, 0.f, 0.f));
			//this->SetActorRelativeRotation(FRotator(0.f, 0.f, 1 - Bearing * 15.f));

			//SetActorLocation(FMath::InterpCircularInOut(GetActorLocation(), MoveTarget, DeltaTime * 5));

			//SetActorLocation(FMath::VInterpTo(GetActorLocation(), MoveTarget, DeltaTime, 0.5f));
			//SetActorRelativeLocation(GetActorForwardVector()*DeltaTime*MaxSpeed);

		//SetActorRelativeRotation(FRotator(0.f, 0.f, FMath::Clamp(Bearing / 2, -45.f, 45.f)))
			//this->springArm->TargetArmLength = FMath::Lerp(this->springArm->TargetArmLength, DesiredCamZoom, DeltaTime*5.f);
	}
}


void ABaseMobileSpaceUnit::StartMoveTo(FVector targetLoc)
{
	MoveTarget = targetLoc;

	FVector Dir = (MoveTarget - GetActorLocation());
	Dir.Normalize();
	FVector Forward = GetActorForwardVector();
	Forward.Normalize();
	float Dist = FVector::Dist(GetActorLocation(), MoveTarget);
	float Dot = FVector::DotProduct(Dir, Forward);
	float Bearing = FQuat::FindBetweenVectors(Forward, Dir).Rotator().Yaw;
	float UnitRadius = this->RootComponent->Bounds.SphereRadius;
	if (Dist <= UnitRadius)
	{
		MoveType = EMoveTypeEnum::TurnOnly;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MoveType: TurnOnly");
		return;
	}
	if (Dist <= (UnitRadius * 3) || FMath::Abs(Bearing) >= 45)
	{
		MoveType = EMoveTypeEnum::TurnThanGo;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MoveType: TurnTHANGo");
		return;
	}
	if (FMath::Abs(Bearing) < 90 && Dist > UnitRadius)
	{
		MoveType = EMoveTypeEnum::TurnAndGo;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MoveType: TurnANDGo");
	}
}