// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSPlayerController.h"
#include "MainALSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AMainALSPlayerController : public AALSPlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* NewPawn) override;

	virtual void OnRep_Pawn() override;

	virtual void BeginPlayingState() override;
	
};
