// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceUnitHardpoint.h"
#include "ProjectileBase.h"
#include "SpaceUnitWeaponHardpoint.generated.h"

/**
 *
 */
UCLASS()
class SWWR_API ASpaceUnitWeaponHardpoint : public ASpaceUnitHardpoint
{
	GENERATED_BODY()

public:
	ASpaceUnitWeaponHardpoint();

	virtual void CustomTick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void SetTarget(FVector loc);
	void ClearTarget();

	UPROPERTY(EditAnywhere, Category = "Attributes")
		FString ProjectileActorBPPath;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		TSubclassOf<class AProjectileBase> ProjectileBP;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		float Damage = 1.f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		float FireInterval = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		UAudioComponent* FireSound;


	UPROPERTY(EditAnywhere, Category = "Attributes")
		UPointLightComponent* MuzzleLight;
	//SoundWave'/Game/Audio/GUN_SUPERLASER.GUN_SUPERLASER'

	float FireTimer = 0.f;

	FVector TargetLoc;
	bool bIsAttacking = false;

	FTimerHandle FlashResetTimer;

	float OriginalFlashIntensity = 0.f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		float LeftArcExtent = 45.f;
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float RightArcExtent = 45.f;
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float TopArcExtent = 45.f;
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float BottomArcExtent = 45.f;
	void ResetFlash();
};
