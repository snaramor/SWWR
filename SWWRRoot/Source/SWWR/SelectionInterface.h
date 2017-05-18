// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SelectionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class USelectionInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 *
 */
class SWWR_API ISelectionInterface
{
	GENERATED_IINTERFACE_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
		void OnSelected(ASWWRPlayerController* selector);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
		void OnDeselected();

};
