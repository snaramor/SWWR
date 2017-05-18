// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "GridRevealMaster.h"
#include "BaseSpaceUnit.h"

// Sets default values
AGridRevealMaster::AGridRevealMaster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	testMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Mesh"));
	testMesh->SetupAttachment(root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshBox(TEXT("StaticMesh'/Game/Models/GridPlane.GridPlane'"));
	if (StaticMeshBox.Object)
		testMesh->SetStaticMesh(StaticMeshBox.Object);
	// and finally register the static mesh component
	testMesh->RegisterComponent();

}

// regenerate instance (in editor) when necessary
void AGridRevealMaster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CreateTexture(false);
	UpdateTexture();
}


// Called when the game starts or when spawned
void AGridRevealMaster::BeginPlay()
{
	Super::BeginPlay();

}

void AGridRevealMaster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CreateTexture(true);
	UpdateTexture();
}

// Called every frame
void AGridRevealMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float time = GetGameTimeSinceCreation();
	if ((int)time % 2 == 0)
	{
		UpdateTexture();
	}
}


void AGridRevealMaster::CreateTexture(bool forceMake)
{
	UE_LOG(LogTemp, Warning, TEXT("---------- Creating Texture! ----------"));

	// check to see if we actually need to make all this from scratch
	if (dtMaterialInstanceDynamic == nullptr || dtTexture == nullptr || forceMake == true)
	{
		// dynamic texture properties (hard wired here for now)
		dtBytesPerPixel = 4;
		dtWidth = 2048;
		dtHeight = 2048;
		// create buffers to collate pixel data into
		dtBufferSize = dtWidth * dtHeight * dtBytesPerPixel;
		dtBufferSizeSqrt = dtWidth * dtBytesPerPixel;
		dtBuffer = new uint8[dtBufferSize]; // this is the data that we Memcpy into the dynamic texture
		// Create dynamic material
		dtMaterialInstanceDynamic = testMesh->CreateAndSetMaterialInstanceDynamic(0);
		// create dynamic texture
		dtTexture = UTexture2D::CreateTransient(dtWidth, dtHeight);
		dtTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		dtTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		dtTexture->SRGB = 0;
		dtTexture->AddToRoot();          // Guarantee no garbage collection by adding it as a root reference
		dtTexture->UpdateResource(); // Update the texture with new variable values.
		// plug the dynamic texture into the dynamic material
		dtMaterialInstanceDynamic->SetTextureParameterValue(FName("DynamicTexture"), dtTexture);
		// Create a new texture region with the width and height of our dynamic texture
		dtUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, dtWidth, dtHeight);
	}

}




void AGridRevealMaster::UpdateTexture()
{
	//UE_LOG(LogTemp, Warning, TEXT("---------- Updating Texture! ----------"));

	int pixelcount = dtBufferSize / dtBytesPerPixel;

	for (int i = 0; i < pixelcount; i++)
	{

		int iBlue = i * 4 + 0;
		int iGreen = i * 4 + 1;
		int iRed = i * 4 + 2;
		int iAlpha = i * 4 + 3;
		int iX = i % dtWidth;
		int iY = i / dtWidth;
		dtBuffer[iBlue] = (iX < dtWidth) ? 0 : 255;//FMath::Clamp(iX / 20, 0, 255);
		dtBuffer[iGreen] = (iX < dtWidth) ? 0 : 255;//FMath::Clamp(iX / 20, 0, 255);
		dtBuffer[iRed] = (iX < dtWidth) ? 0 : 255;//FMath::Clamp(iX / 20, 0, 255);
		dtBuffer[iAlpha] = 255;

	}

	/*
	for (TActorIterator<ABaseSpaceUnit> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ABaseSpaceUnit *unit = *ActorItr;
		FVector Pos = unit->GetActorLocation();
		float Ratio = dtWidth / 25000;
		FVector2D RelPos = FVector2D(Ratio * Pos.X, Ratio * Pos.Y);
	}*/
	UpdateTextureRegions(dtTexture, 0, 1, dtUpdateTextureRegion, dtBufferSizeSqrt, (uint32)4, dtBuffer, false);
	dtMaterialInstanceDynamic->SetTextureParameterValue("DynamicTextureParam", dtTexture);
}





void AGridRevealMaster::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}

		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}