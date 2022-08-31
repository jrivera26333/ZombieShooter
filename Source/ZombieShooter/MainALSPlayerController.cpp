// Fill out your copyright notice in the Description page of Project Settings.


#include "MainALSPlayerController.h"
#include "MainALSCharacter.h"

void AMainALSPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	//PossessedCharacter = Cast<AALSBaseCharacter>(NewPawn);
	//SetupDebugInputs();
}

void AMainALSPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
}

void AMainALSPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
}