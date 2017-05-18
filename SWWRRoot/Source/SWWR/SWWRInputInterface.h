// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SWWRInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class USWWRInputInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 *
 */
class SWWR_API ISWWRInputInterface
{
	GENERATED_IINTERFACE_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
		void OnInputClick();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
		void OnInputHold();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
		void OnInputHoldReleased(float holdTime);


};
