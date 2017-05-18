// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "SWWRHud.h"
#include "Blueprint/UserWidget.h"
#include "BaseSpaceUnit.h"
#include "SWWRPlayerController.h"
#include "EngineUtils.h"

ASWWRHud::ASWWRHud()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


	FString Path = "Texture2D'/Game/Textures/UI/grid.grid'";
	MapBG = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(Path)));

}
void ASWWRHud::BeginPlay()
{
	//ASWWRCameraPawn* MyPawn = Cast<ASWWRCameraPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (BaseHudClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), BaseHudClass);

		if (CurrentWidget != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Adding widget to viewport"));
			CurrentWidget->AddToViewport();
		}
	}
	MyController = Cast<ASWWRPlayerController>(GetOwningPlayerController());
	int32 ScrW;
	int32 ScrH;
	MyController->GetViewportSize(ScrW, ScrH);
	float width = ScrW*0.15f;
	FVector2D TopLeft = FVector2D(0, ScrH - width);


	//this->AddHitBox(TopLeft, FVector2D(width, width), FName(TEXT("Minimap")), true);
}

void ASWWRHud::DrawHUD()
{
	DrawMinimap();
	if (MyController->bIsHolding && this->CurrentWidget->IsHovered() == false)
	{
		FVector2D InitialClickPos = MyController->InitialClickPos;
		MyController->GetMousePosition(MouseLocation.X, MouseLocation.Y);
		DrawRect(FLinearColor(1.f, 1.f, 1.f, 0.1f), InitialClickPos.X, InitialClickPos.Y, -(InitialClickPos.X - MouseLocation.X), -(InitialClickPos.Y - MouseLocation.Y));
		DrawLine(InitialClickPos.X, InitialClickPos.Y, MouseLocation.X, InitialClickPos.Y, lineColor, lineThickness);
		DrawLine(MouseLocation.X, InitialClickPos.Y, MouseLocation.X, MouseLocation.Y, lineColor, lineThickness);
		DrawLine(MouseLocation.X, MouseLocation.Y, InitialClickPos.X, MouseLocation.Y, lineColor, lineThickness);
		DrawLine(InitialClickPos.X, MouseLocation.Y, InitialClickPos.X, InitialClickPos.Y, lineColor, lineThickness);

		unitsUnderSelection.Empty();

		GetActorsInSelectionRectangle<ABaseSpaceUnit>(InitialClickPos, MouseLocation, unitsUnderSelection, false, true);
	}
}

void ASWWRHud::DrawMinimap() {
	//FVector2D ScreenSize;
	//TODO: Minimap size is wierd?
	int32 ScrW;
	int32 ScrH;
	MyController->GetViewportSize(ScrW, ScrH);
	float width = ScrW*0.15f;
	FVector2D TopLeft = FVector2D(0, ScrH - width);
	FVector2D BottomRight = FVector2D(width, ScrH);
	this->DrawTexture(MapBG, TopLeft.X, TopLeft.Y, width, width, 0, 0, width, width);
	//DrawLine(TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y, FLinearColor::White, 1.f);
	FVector Pos = MyController->GetPawn()->GetActorLocation();
	float Ratio = width / 25000;
	FVector2D RelPos = FVector2D(Ratio * Pos.X, Ratio * Pos.Y);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RelPos: ") + RelPos.ToString());
	DrawRect(FLinearColor::Blue, RelPos.X - 5, (ScrH - width) + RelPos.Y - 5, 10, 10);
	//this->RenderHitBoxes(this->Canvas->Canvas);
	//this->NotifyHitBoxClick()
	for (TActorIterator<ABaseSpaceUnit> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ABaseSpaceUnit *unit = *ActorItr;
		Pos = unit->GetActorLocation();
		RelPos = FVector2D(Ratio * Pos.X, Ratio * Pos.Y);
		FLinearColor Col = FLinearColor::White;
		if (unit->bIsSelected)
		{
			Col = FLinearColor::Green;
		}
		DrawRect(Col, RelPos.X - 1, (ScrH - width) + RelPos.Y - 1, 2, 2);
	}
}

void ASWWRHud::DelegateMinimapClick() {
	//if (BoxName == FName(TEXT("Minimap")))
	//{
	int32 ScrW = 0.f;
	int32 ScrH = 0.f;
	MyController->GetViewportSize(ScrW, ScrH);
	float width = ScrW*0.15f;
	float Ratio = 25000 / width;
	FVector2D TopLeft = FVector2D(0, ScrH - width);
	FVector2D BottomRight = FVector2D(width, ScrH);
	MyController->GetMousePosition(MouseLocation.X, MouseLocation.Y);
	float CalcY = MouseLocation.Y - (ScrH - width);
	FVector2D RPos = FVector2D(MouseLocation.X, CalcY);// ((MouseLocation.Y - ScrH) + width)*Ratio);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ratio: ") + FString::SanitizeFloat(Ratio) + RPos.ToString());
	MyController->GetPawn()->SetActorLocation(FVector(RPos.X*Ratio, RPos.Y*Ratio, 0));
	//MyController->GetPawn()->location
	//this->GetHitBoxWithName(BoxName)->
//}
}

void ASWWRHud::ReceiveDrawHUD(int32 SizeX, int32 SizeY) {

}


void ASWWRHud::UpdateDisplayedUnitList() {
	UFunction* updatefunc = this->CurrentWidget->FindFunction(FName("UpdateUnitList"));
	if (updatefunc)
	{
		CurrentWidget->ProcessEvent(updatefunc, nullptr);
	}
}









/*

void ASWWRHud::StartClick(FVector2D pos)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HUD Start Click"));

	InitialClickPos = pos;
	bIsHolding = false;
}
void ASWWRHud::EndClick(FVector2D pos)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HUD End Click"));
	GetWorld()->GetTimerManager().ClearTimer(ClickHoldHandle);
	FinalClickPos = pos;
	if (bIsHolding)
	{
		bIsHolding = false;
	}
	else {
		FHitResult hitResult = FHitResult(ForceInit);
		GetOwningPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, false, hitResult);

		if (hitResult.Actor != nullptr)
		{
			if (hitResult.Actor->IsA(ABaseSpaceUnit::StaticClass()))
			{
				ABaseSpaceUnit* unit = Cast<ABaseSpaceUnit>(hitResult.Actor.Get());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unit Clicked"));
				myController->ClearSelectedUnits();
				myController->AddToSelection(unit);
				return;
			}
		}
		myController->ClearSelectedUnits();
	}
}
void ASWWRHud::CheckHold() {
	bIsHolding = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("User is Holding Click"));
}*/