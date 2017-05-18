// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "SWWRGameMode.generated.h"

/**
 *
 */
UCLASS()
class SWWR_API ASWWRGameMode : public AGameMode
{
	GENERATED_BODY()

		virtual void BeginPlay() override;

public:
	ASWWRGameMode();


};
