// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESHOOTER_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = Health)
	void OnInteract(); //This is the Blueprint Implementation

		//This will be the C++ Implementation
	virtual void OnInteract_Implementation() = 0; //By using = 0 we are telling the compiler its an interface and will be defined in a derived class

	UFUNCTION(BlueprintNativeEvent, Category = Health)
	void OnCompleteInteract(); //This is the Blueprint Implementation

			//This will be the C++ Implementation
	virtual void OnCompleteInteract_Implementation() = 0; //By using = 0 we are telling the compiler its an interface and will be defined in a derived class
};
