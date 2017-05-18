// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseSpaceUnit.h"
#include "SpaceUnitHardpoint.generated.h"

UCLASS()
class SWWR_API ASpaceUnitHardpoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpaceUnitHardpoint();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame

	virtual void CustomTick(float DeltaSeconds);

	ABaseSpaceUnit* ParentShip;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float HardpointHealth = 100.f;


	USceneComponent* dummy;
};
