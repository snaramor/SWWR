// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "SWWRPlayerController.h"
#include "SWWRHud.h"
#include "BaseSpaceUnit.h"
#include "Blueprint/UserWidget.h"
#include "BaseMobileSpaceUnit.h"

ASWWRPlayerController::ASWWRPlayerController() {
	PrimaryActorTick.bCanEverTick = true;
	this->bShowMouseCursor = true;
	this->bEnableMouseOverEvents = true;
	this->bEnableClickEvents = true;
	this->bEnableTouchEvents = false;
	this->bEnableTouchOverEvents = false;
	this->EnableInput(this);
	this->DefaultClickTraceChannel = ECollisionChannel::ECC_WorldDynamic;
	this->bBlockInput = false;
	/*FInputModeGameAndUI inputMode;
	inputMode.SetHideCursorDuringCapture(false);
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	this->SetInputMode(inputMode);*/
}
void ASWWRPlayerController::BeginPlay() {
	Super::BeginPlay();
	CustomHUD = Cast<ASWWRHud>(MyHUD);
}
// Called to bind functionality to input

void ASWWRPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Controller Tick"));


	/*if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start Click"));
		StartClick();
	}*/



	FHitResult hitResult = FHitResult(ForceInit);
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, hitResult);
	if (hitResult.Actor != nullptr)
	{
		if (hitResult.Actor != nullptr)
		{
			if (hitResult.Actor->IsA(ABaseSpaceUnit::StaticClass()))
			{
				ABaseSpaceUnit* unit = Cast<ABaseSpaceUnit>(hitResult.Actor.Get());
				if (unit->TeamNum != TeamNum)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Found"));
					//this->CurrentWidget->SetCursor(EMouseCursor::Crosshairs);
					if (SelectedUnits.Num() > 0)
					{
						CurrentMouseCursor = EMouseCursor::Crosshairs;
					}
				}
				else {
					//this->CurrentWidget->SetCursor(EMouseCursor::Default);
					CurrentMouseCursor = EMouseCursor::Default;
				}
			}
			else {
				CurrentMouseCursor = EMouseCursor::Default;
			}
		}
		else {
			CurrentMouseCursor = EMouseCursor::Default;
		}
	}
}


void ASWWRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Select", IE_Pressed, this, &ASWWRPlayerController::StartClick);
	InputComponent->BindAction("Select", IE_Released, this, &ASWWRPlayerController::EndClick);
	//FInputActionBinding act;
	//act.ActionName = FName("Select");
	//FInputActionUnifiedDelegate deleg;
	//deleg.BindDelegate(this, FName("StartClick"));
	//act.ActionDelegate = deleg;
	//InputComponent->AddActionBinding(act);
	InputComponent->BindAction("Command", IE_Released, this, &ASWWRPlayerController::Command);

}
void ASWWRPlayerController::Command() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Command"));
	FHitResult hitResult = FHitResult(ForceInit);
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, hitResult);

	if (hitResult.Actor != nullptr && hitResult.Actor->IsA(ABaseSpaceUnit::StaticClass()))
	{
	}
	else {
		//didnt hit an actor, run a trace to the grid for move command
		GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, hitResult);
		if (hitResult.Actor != nullptr)
		{
			DrawDebugSphere(GetWorld(), FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, 0.f), 24, 32, FColor(255, 0, 0), false, 2.f);
			for (ABaseSpaceUnit* unit : SelectedUnits)
			{
				if (unit->IsA(ABaseMobileSpaceUnit::StaticClass()))
				{
					if (this->IsInputKeyDown(EKeys::LeftControl))
					{
						Cast<ABaseMobileSpaceUnit>(unit)->AttackLocation(hitResult.ImpactPoint);
					}
					else
					{
						Cast<ABaseMobileSpaceUnit>(unit)->StartMoveTo(hitResult.ImpactPoint);
						Cast<ABaseMobileSpaceUnit>(unit)->StopAttacking();
					}
				}
			}
		}
	}
	//ClearSelectedUnits();
}


void ASWWRPlayerController::StartClick() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start Click"));
	float mX;
	float mY;
	GetMousePosition(mX, mY);
	InitialClickPos = FVector2D(mX, mY);
	bIsHolding = false;
	GetWorld()->GetTimerManager().SetTimer(ClickHoldHandle, this, &ASWWRPlayerController::CheckHold, MaxClickHoldTimer, false);
}


void ASWWRPlayerController::EndClick() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("End Click"));
	GetWorld()->GetTimerManager().ClearTimer(ClickHoldHandle);
	UUserWidget* widget = CustomHUD->CurrentWidget;
	if (widget->IsHovered())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("We clicked UI, cancel trace check"));
		return;
	}
	float mX;
	float mY;
	GetMousePosition(mX, mY);
	FinalClickPos = FVector2D(mX, mY);
	if (bIsHolding)
	{
		bIsHolding = false;
		if (CustomHUD != nullptr)
		{

			if (CustomHUD->unitsUnderSelection.Num() > 0)
			{
				TArray<ABaseSpaceUnit*> foundUnits = CustomHUD->unitsUnderSelection;

				if (IsInputKeyDown(EKeys::LeftControl))
				{
					for (ABaseSpaceUnit* unit : foundUnits)
					{
						if (SelectedUnits.Contains(unit))
						{
							RemoveFromSelection(unit);
						}
						else {
							if (unit->TeamNum != this->TeamNum)
								continue;
							AddToSelection(unit);
						}
					}
				}
				else {
					ClearSelectedUnits();
					for (ABaseSpaceUnit* unit : CustomHUD->unitsUnderSelection)
					{
						if (unit->TeamNum != this->TeamNum)
							continue;
						AddToSelection(unit);
					}
					//AddToSelection(CustomHUD->unitsUnderSelection);
				}

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Found ") + FString::FromInt(CustomHUD->unitsUnderSelection.Num()) + TEXT(" units"));

			}

		}

	}
	else {
		FHitResult hitResult = FHitResult(ForceInit);
		GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, hitResult);

		if (hitResult.Actor != nullptr)
		{
			if (hitResult.Actor->IsA(ABaseSpaceUnit::StaticClass()))
			{
				ABaseSpaceUnit* unit = Cast<ABaseSpaceUnit>(hitResult.Actor.Get());
				if (unit->TeamNum != this->TeamNum)
					return;
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unit Clicked"));
				if (IsInputKeyDown(EKeys::LeftControl))
				{
					if (SelectedUnits.Contains(unit))
					{
						RemoveFromSelection(unit);
					}
					else {
						AddToSelection(unit);
					}
				}
				else {
					ClearSelectedUnits();
					AddToSelection(unit);
				}
				return;
			}
		}
		if (!IsInputKeyDown(EKeys::LeftControl)) {
			ClearSelectedUnits();
		}
	}
}


void ASWWRPlayerController::CheckHold() {
	bIsHolding = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("User is Holding Click"));
}


void ASWWRPlayerController::ClearSelectedUnits()
{
	for (ABaseSpaceUnit* unit : SelectedUnits)
	{
		ISelectionInterface* interface = Cast<ISelectionInterface>(unit);
		if (interface != nullptr)
		{
			interface->Execute_OnDeselected(unit);
		}
	}
	SelectedUnits.Empty();
	CustomHUD->UpdateDisplayedUnitList();
}


void ASWWRPlayerController::AddToSelection(ABaseSpaceUnit* unit)
{
	ISelectionInterface* interface = Cast<ISelectionInterface>(unit);
	if (interface != nullptr)
	{
		interface->Execute_OnSelected(unit, this);
		SelectedUnits.AddUnique(unit);
		CustomHUD->UpdateDisplayedUnitList();
	}
}


void ASWWRPlayerController::AddToSelection(TArray<ABaseSpaceUnit *> units)
{
	for (ABaseSpaceUnit* unit : units)
	{
		ISelectionInterface* interface = Cast<ISelectionInterface>(unit);
		if (interface != nullptr)
		{
			interface->Execute_OnSelected(unit, this);
		}
		SelectedUnits.AddUnique(unit);
	}
	CustomHUD->UpdateDisplayedUnitList();
}


void ASWWRPlayerController::RemoveFromSelection(ABaseSpaceUnit* unit) {
	ISelectionInterface* interface = Cast<ISelectionInterface>(unit);
	if (interface != nullptr)
	{
		interface->Execute_OnDeselected(unit);
	}
	SelectedUnits.Remove(unit);
	CustomHUD->UpdateDisplayedUnitList();
}


void ASWWRPlayerController::RemoveFromSelection(TArray<ABaseSpaceUnit *> units)
{
	for (ABaseSpaceUnit* unit : units)
	{
		ISelectionInterface* interface = Cast<ISelectionInterface>(unit);
		if (interface != nullptr)
		{
			interface->Execute_OnDeselected(unit);
		}
		SelectedUnits.Remove(unit);
	}
	CustomHUD->UpdateDisplayedUnitList();
}