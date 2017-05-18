// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "BaseSpaceUnit.h"
#include "SWWRPlayerController.h"
#include "SpaceUnitHardpoint.h"
#include "SpaceUnitWeaponHardpoint.h"

// Sets default values
ABaseSpaceUnit::ABaseSpaceUnit()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultMesh"));
	SelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionCircle"));
	RootComponent = mesh;

	SelectionCircle->SetupAttachment(mesh);


	FString IconPath = "Texture2D'/Game/Textures/imp_logo.imp_logo'";
	DefaultIconTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(IconPath)));


	FString DiscPath = "StaticMesh'/Game/Models/planardisc.planardisc'";
	SelectionCircle->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *(DiscPath))));
}

// Called when the game starts or when spawned
void ABaseSpaceUnit::BeginPlay()
{
	Super::BeginPlay();


	if (SelectionCircle != nullptr)
	{
		FVector Bounds;
		FVector BoundsOrigin;
		Bounds = FVector(mesh->Bounds.SphereRadius);
		SelectionCircle->SetWorldScale3D(Bounds);
		SelectionCircle->SetHiddenInGame(true);

	}


	SetDefaultIcon();
	SetDefaultValues();

	if (mesh != nullptr)
	{
		mesh->OnBeginCursorOver.AddDynamic(this, &ABaseSpaceUnit::StartCursorOver);
		mesh->OnEndCursorOver.AddDynamic(this, &ABaseSpaceUnit::EndCursorOver);
		for (int i = 0; i < mesh->GetNumMaterials(); i++)
		{
			if (mesh->GetMaterial(i) != nullptr)
			{
				MyMatInstances.Add(mesh->CreateDynamicMaterialInstance(i));
			}
		}
	}

	TArray<AActor*> children;
	GetAllChildActors(children);
	for (AActor* child : children)
	{
		if (child->IsA(ASpaceUnitWeaponHardpoint::StaticClass()))
		{
			ASpaceUnitWeaponHardpoint* hardpoint = Cast<ASpaceUnitWeaponHardpoint>(child);
			WeaponPoints.Add(hardpoint);
		}
	}


}

void ABaseSpaceUnit::SetDefaultValues()
{
	CurrentHealth = MaxHealth;
	CurrentShield = MaxShield;
	bIsSelected = false;
}

void ABaseSpaceUnit::SetDefaultIcon()
{
	if (IconTexture == nullptr && DefaultIconTexture != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Icon is nullptr, resorting to default"));
		IconTexture = DefaultIconTexture;
	}
}


void ABaseSpaceUnit::StartCursorOver(UPrimitiveComponent* Component) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cursor Hover Start"));
	//if(Cast<ASWWRPlayerController*>(this->control))
	for (UMaterialInstanceDynamic* mat : MyMatInstances)
	{
		mat->SetScalarParameterValue(FName("EmissiveAdd"), 0.1f);
	}
	//TODO: Show Hardpoint Icons
}


void ABaseSpaceUnit::EndCursorOver(UPrimitiveComponent* Component) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cursor Hover End"));
	for (UMaterialInstanceDynamic* mat : MyMatInstances)
	{
		mat->SetScalarParameterValue(FName("EmissiveAdd"), 0.f);
	}
	//TODO: Hide Hardpoint Icons
}


void ABaseSpaceUnit::SetTeam(uint8 inTeam)
{
	this->TeamNum = inTeam;
}


uint8 ABaseSpaceUnit::GetTeam()
{
	return TeamNum;
}


// Called every frame
void ABaseSpaceUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor*> children;
	this->GetAllChildActors(children);
	for (AActor* child : children)
	{
		if (child->IsA(ASpaceUnitHardpoint::StaticClass()))
		{
			ASpaceUnitHardpoint* hardpoint = Cast<ASpaceUnitHardpoint>(child);
			hardpoint->CustomTick(DeltaTime);
		}
	}
}




void ABaseSpaceUnit::OnSelected_Implementation(ASWWRPlayerController* selector)
{
	bIsSelected = true;
	if (selector->TeamNum != this->TeamNum)
		return;
	if (SelectionCircle != nullptr)
		SelectionCircle->SetHiddenInGame(false);
	//TODO: Play selected audio
}
void ABaseSpaceUnit::OnDeselected_Implementation() {
	bIsSelected = false;
	if (SelectionCircle != nullptr)
		SelectionCircle->SetHiddenInGame(true);
}

void ABaseSpaceUnit::AttackLocation(FVector loc)
{
	for (ASpaceUnitWeaponHardpoint* weap : WeaponPoints)
	{
		weap->SetTarget(loc);
	}
}
void ABaseSpaceUnit::StopAttacking()
{
	for (ASpaceUnitWeaponHardpoint* weap : WeaponPoints)
	{
		weap->ClearTarget();
	}
}