// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "SpaceUnitWeaponHardpoint.h"


ASpaceUnitWeaponHardpoint::ASpaceUnitWeaponHardpoint()
{
	//ProjectileBP = (UClass*)Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *(ProjectileActorBPPath)))->GeneratedClass;
	FireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("FireSound"));
	FireSound->SetupAttachment(RootComponent);
	FireSound->bAlwaysPlay = false;
	FireSound->bAutoActivate = false;

	MuzzleLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MuzzleFlash"));

	FlushPersistentDebugLines(GetWorld());

	FColor col = FColor::Cyan;
	float lineThickness = 1.0f;
	FVector root;
	FVector dir;
	if (this->IsChildActor())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Child Actor Constructor"));
		root = this->dummy->ComponentToWorld.GetLocation();
		dir = this->dummy->GetForwardVector();
	}
	else
	{
		root = GetActorLocation();
		dir = GetActorForwardVector();
	}

	FVector BottomLeftPt = root + (FRotator(-BottomArcExtent, -LeftArcExtent, 0.f).RotateVector(dir*250.f));
	DrawDebugLine(GetWorld(), root, BottomLeftPt, col, true, -1.0f, 0, lineThickness);

	FVector TopLeftPt = root + (FRotator(TopArcExtent, -LeftArcExtent, 0.f).RotateVector(dir*250.f));
	DrawDebugLine(GetWorld(), root, TopLeftPt, col, true, -1.0f, 0, lineThickness);

	FVector TopRightPt = root + (FRotator(TopArcExtent, RightArcExtent, 0.f).RotateVector(dir*250.f));
	DrawDebugLine(GetWorld(), root, TopRightPt, col, true, -1.0f, 0, lineThickness);

	FVector BottomRightPt = root + (FRotator(-BottomArcExtent, RightArcExtent, 0.f).RotateVector(dir*250.f));
	DrawDebugLine(GetWorld(), root, BottomRightPt, col, true, -1.0f, 0, lineThickness);

	DrawDebugLine(GetWorld(), BottomLeftPt, TopLeftPt, col, true, -1.0f, 0, lineThickness);
	DrawDebugLine(GetWorld(), TopLeftPt, TopRightPt, col, true, -1.0f, 0, lineThickness);
	DrawDebugLine(GetWorld(), TopRightPt, BottomRightPt, col, true, -1.0f, 0, lineThickness);
	DrawDebugLine(GetWorld(), BottomRightPt, BottomLeftPt, col, true, -1.0f, 0, lineThickness);
}

void ASpaceUnitWeaponHardpoint::BeginPlay()
{
	Super::BeginPlay();
	OriginalFlashIntensity = MuzzleLight->Intensity;
	MuzzleLight->SetIntensity(0.f);
}

void ASpaceUnitWeaponHardpoint::SetTarget(FVector loc)
{
	bIsAttacking = true;
	TargetLoc = loc;
	FireTimer = FMath::RandRange(0.f, 1.f);
}
void ASpaceUnitWeaponHardpoint::ClearTarget()
{
	bIsAttacking = false;
}
void ASpaceUnitWeaponHardpoint::CustomTick(float DeltaTime)
{
	if (bIsAttacking)
	{
		if (FireTimer <= 0)
		{
			FireTimer = FireInterval + FMath::RandRange(-0.5f, 0.5f);
			FActorSpawnParameters params;
			params.Instigator = this->ParentShip;
			FRotator rot = (TargetLoc - GetActorLocation()).Rotation();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Firing"));
			AProjectileBase* proj = GetWorld()->SpawnActor<AProjectileBase>(ProjectileBP, this->GetActorLocation() + FVector(0, 0, 2.f), rot, params);
			proj->SetLifeSpan(3.f);

			if (FireSound->Sound != nullptr)
				FireSound->Play(0.f);

			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound->Sound, GetActorLocation(), 0.25f);

			MuzzleLight->SetIntensity(OriginalFlashIntensity);
			GetWorld()->GetTimerManager().SetTimer(FlashResetTimer, this, &ASpaceUnitWeaponHardpoint::ResetFlash, 0.2f, false);
		}
	}
	if (FireTimer > 0)
	{
		FireTimer = FMath::Clamp(FireTimer - DeltaTime, 0.f, 100.f);
	}
}
void ASpaceUnitWeaponHardpoint::ResetFlash()
{
	MuzzleLight->SetIntensity(0.f);
}