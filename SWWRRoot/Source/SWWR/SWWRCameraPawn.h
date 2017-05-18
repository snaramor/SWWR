// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BaseSpaceUnit.h"
#include "SWWRCameraPawn.generated.h"

UCLASS()
class SWWR_API ASWWRCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASWWRCameraPawn();

	/*Accessor function for selected units*/
	/*UFUNCTION(BlueprintPure, Category = "Selection")
		TArray<ABaseSpaceUnit*> GetAllSelectedUnits();

	UFUNCTION(BlueprintPure, Category = "Selection")
		bool ClearSelectedUnits();

	UFUNCTION(BlueprintCallable, Category = "Selection")
		void AddToSelection(ABaseSpaceUnit* unitToAdd);
		*/
		// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void MoveForward(float axisForward);
	void MoveRight(float axisRight);

	void ZoomIn();
	void ZoomOut();
	UPROPERTY(EditAnywhere)
		USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere)
		UCameraComponent* camera;

	float MinCamZoom = 50.f;
	float MaxCamZoom = 1800.f;
	float CurrentCamZoom = 600.f;

	UPROPERTY(EditAnywhere)
		float DesiredCamZoom = 600.f;

	float CamZoomSpeed = 40.f;
	float CamPanSpeed = 100.f;

	float MoveHor;
	float MoveVert;
};
