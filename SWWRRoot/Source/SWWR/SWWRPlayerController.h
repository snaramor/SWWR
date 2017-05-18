// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BaseSpaceUnit.h"
#include "SWWRHud.h"
#include "SWWRPlayerController.generated.h"

UCLASS()
class SWWR_API ASWWRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASWWRPlayerController();

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void StartClick();
	void EndClick();
	void Command();

	void ClearSelectedUnits();
	void AddToSelection(ABaseSpaceUnit* unit);
	void AddToSelection(TArray<ABaseSpaceUnit*> units);
	void RemoveFromSelection(ABaseSpaceUnit* unit);
	void RemoveFromSelection(TArray<ABaseSpaceUnit*> units);

	void CheckHold();
	bool bIsHolding = false;

	FVector2D MouseLocation;

	float ClickHoldTimer = 0.f;

	float MaxClickHoldTimer = 0.1f;

	FTimerHandle ClickHoldHandle;

	FVector2D InitialClickPos;

	FVector2D FinalClickPos;

	UPROPERTY(EditAnywhere)
		uint8 TeamNum = 1;

	//ASWWRHud* CustomHUD;


	ASWWRHud* CustomHUD;



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<ABaseSpaceUnit*> SelectedUnits;
};
