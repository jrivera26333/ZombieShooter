// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Attack.h"
#include "ZombieAIController.h"
#include "ZombieShooterCharacter.h"


UMyBTTask_Attack::UMyBTTask_Attack()
{
	NodeName = TEXT("Melee");
}

EBTNodeResult::Type UMyBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//From the controller we get the character which has the Shoot()
	AZombieShooterCharacter* Character = Cast<AZombieShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//Character->Shoot();
	return EBTNodeResult::Succeeded;
}