// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "BaseSpaceUnit.h"
//#include "SWWRPlayerController.h"
#include "SWWRHud.generated.h"

class ASWWRPlayerController;

UCLASS()
class SWWR_API ASWWRHud : public AHUD
{
	GENERATED_BODY()

public:
	ASWWRHud();

	virtual void DrawHUD();

	virtual void ReceiveDrawHUD(int32 SizeX, int32 SizeY);

	virtual void BeginPlay() override;

	//void NotifyHitBoxClick(const FName BoxName) override;

	UFUNCTION(BlueprintCallable, Category = "Minimap")
		void DelegateMinimapClick();

	void UpdateDisplayedUnitList();

	FVector2D MouseLocation;

	void DrawMinimap();

	UTexture2D* MapBG = nullptr;

	//ASWWRPlayerController* myController;

	ASWWRPlayerController* MyController;

	FLinearColor lineColor = FLinearColor::White;
	float lineThickness = 1.f;

	TArray<ABaseSpaceUnit*> unitsUnderSelection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UUserWidget* CurrentWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> BaseHudClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> PauseHudClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> EmptyHudClass;
};
