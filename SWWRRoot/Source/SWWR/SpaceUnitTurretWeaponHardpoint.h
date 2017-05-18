// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceUnitWeaponHardpoint.h"
#include "SpaceUnitTurretWeaponHardpoint.generated.h"

/**
 *
 */
UCLASS()
class SWWR_API ASpaceUnitTurretWeaponHardpoint : public ASpaceUnitWeaponHardpoint
{
	GENERATED_BODY()

public:
	ASpaceUnitTurretWeaponHardpoint();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void CustomTick(float DeltaSeconds) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		UStaticMeshComponent* TurretTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		UStaticMeshComponent* TurretBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float AimSpeed = 1.f;

};
