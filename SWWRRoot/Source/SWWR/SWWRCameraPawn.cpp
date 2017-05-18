// Fill out your copyright notice in the Description page of Project Settings.

#include "SWWR.h"
#include "SWWRCameraPawn.h"


// Sets default values
ASWWRCameraPawn::ASWWRCameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = springArm;

	//camera->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	camera->SetupAttachment(RootComponent);
	camera->PostProcessSettings.MotionBlurMax = 0.f;
	camera->PostProcessSettings.MotionBlurAmount = 0.f;
	camera->PostProcessSettings.MotionBlurPerObjectSize = 0.f;
	camera->PostProcessSettings.AntiAliasingMethod = EAntiAliasingMethod::AAM_None;
	camera->PostProcessSettings.AutoExposureLowPercent = 0.3f;
	camera->PostProcessSettings.AutoExposureHighPercent = 0.3f;
	camera->PostProcessSettings.AutoExposureMinBrightness = 0.03f;
	camera->PostProcessSettings.AutoExposureMaxBrightness = 0.03f;
	camera->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Basic;
	camera->PostProcessSettings.AutoExposureSpeedDown = 10.f;
	camera->PostProcessSettings.AutoExposureSpeedUp = 10.f;
	camera->PostProcessSettings.HistogramLogMax = 0.f;
	camera->PostProcessSettings.HistogramLogMin = 0.f;
	springArm->bDoCollisionTest = false;
	springArm->TargetArmLength = 600.f;
	springArm->SetWorldRotation(FRotator(-40.f, -90.f, 0.f));

}

// Called when the game starts or when spawned
void ASWWRCameraPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASWWRCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation() + FVector(MoveHor*(CamPanSpeed + DesiredCamZoom)*DeltaTime, -MoveVert*(CamPanSpeed + DesiredCamZoom)*DeltaTime, 0);

	SetActorLocation(FVector(FMath::Clamp(NewLocation.X, 0.f, 25000.f), FMath::Clamp(NewLocation.Y, 0.f, 25000.f), 0));

	this->springArm->TargetArmLength = FMath::Lerp(this->springArm->TargetArmLength, DesiredCamZoom, DeltaTime*5.f);

}

// Called to bind functionality to input
void ASWWRCameraPawn::SetupPlayerInputComponent(class UInputComponent* inInputComponent)
{
	Super::SetupPlayerInputComponent(inInputComponent);
	inInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ASWWRCameraPawn::ZoomIn);
	inInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ASWWRCameraPawn::ZoomOut);

	inInputComponent->BindAxis("MoveForward", this, &ASWWRCameraPawn::MoveForward);
	inInputComponent->BindAxis("MoveRight", this, &ASWWRCameraPawn::MoveRight);
}

void ASWWRCameraPawn::MoveForward(float axisForward) {
	this->MoveVert = axisForward;
}

void ASWWRCameraPawn::MoveRight(float axisRight) {
	this->MoveHor = axisRight;
}

void ASWWRCameraPawn::ZoomIn() {
	this->DesiredCamZoom = FMath::Clamp(DesiredCamZoom - CamZoomSpeed, MinCamZoom, MaxCamZoom);
}
void ASWWRCameraPawn::ZoomOut() {
	this->DesiredCamZoom = FMath::Clamp(DesiredCamZoom + CamZoomSpeed, MinCamZoom, MaxCamZoom);
}