// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseSpaceUnit.h"
#include "BaseMobileSpaceUnit.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EMoveTypeEnum : uint8 {
	TurnOnly	UMETA(DisplayName = "TurnOnly"),
	TurnThanGo	UMETA(DisplayName = "TurnThanGo"),
	TurnAndGo	UMETA(DisplayName = "TurnAndGo"),
	Finished UMETA(DisplayName = "Finished")
};
UCLASS()
class SWWR_API ABaseMobileSpaceUnit : public ABaseSpaceUnit
{
	GENERATED_BODY()



public:
	ABaseMobileSpaceUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	void StartMoveTo(FVector targetLoc);

	FVector MoveTarget;

	EMoveTypeEnum MoveType = EMoveTypeEnum::Finished;

	FVector TargetPos;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		float MaxSpeed = 50.f;

	float CurrentSpeed = MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		float TurnSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		float MaxBank = 30.f;


};
