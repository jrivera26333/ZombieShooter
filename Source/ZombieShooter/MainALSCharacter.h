// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSCharacter.h"
#include "MainALSCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AMainALSCharacter : public AALSCharacter
{
	GENERATED_BODY()
	
public:
	AMainALSCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
};