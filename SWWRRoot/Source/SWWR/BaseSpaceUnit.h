// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SelectionInterface.h"
#include "BaseSpaceUnit.generated.h"

class ASpaceUnitWeaponHardpoint;

UCLASS()
class SWWR_API ABaseSpaceUnit : public APawn, public ISelectionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseSpaceUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) overr

	virtual void OnSelected_Implementation(ASWWRPlayerController* selector) override;

	virtual void OnDeselected_Implementation() override;

	void SetDefaultIcon();
	void SetDefaultValues();



	void SetTeam(uint8 inTeam);

	uint8 GetTeam();

	UFUNCTION()
		void StartCursorOver(UPrimitiveComponent* Component);

	UFUNCTION()
		void EndCursorOver(UPrimitiveComponent* Component);

	void AttackLocation(FVector loc);

	void StopAttacking();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
		bool bIsSelected;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		uint16 MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		uint16 MaxShield = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FString UnitName = TEXT("Default Name");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FString Description = TEXT("Default Description");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float RevealRange = 100.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
		UTexture2D* IconTexture = nullptr;

	UTexture2D* DefaultIconTexture = nullptr;

	uint16 CurrentHealth = 100;

	uint16 CurrentShield = 100;

	ABaseSpaceUnit* TargetedUnit;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

	UStaticMeshComponent* SelectionCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		uint8 TeamNum = 1;

	TArray<UMaterialInstanceDynamic*> MyMatInstances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		TArray<ASpaceUnitWeaponHardpoint*> WeaponPoints;
};
