// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AZombieAIController : public AAIController
{
	GENERATED_BODY()


public:
	AZombieAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* Behaviour = nullptr;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;
};
