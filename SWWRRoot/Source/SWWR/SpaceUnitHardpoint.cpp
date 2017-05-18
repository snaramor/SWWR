// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "SpaceUnitHardpoint.h"
#include "BaseSpaceUnit.h"


// Sets default values
ASpaceUnitHardpoint::ASpaceUnitHardpoint()
{
	dummy = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = dummy;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpaceUnitHardpoint::BeginPlay()
{
	Super::BeginPlay();
	AActor* parent = this->GetParentActor();
	if (parent != nullptr)
	{
		if (parent->IsA(ABaseSpaceUnit::StaticClass()))
		{
			ParentShip = Cast<ABaseSpaceUnit>(parent);
		}
	}

}

// Called every frame
void ASpaceUnitHardpoint::CustomTick(float DeltaTime)
{

}

