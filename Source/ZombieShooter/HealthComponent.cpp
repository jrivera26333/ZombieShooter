// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "HealthInterface.h"
#include "GameFramework/Actor.h"

//***We created this as a seperate health component because we want to be able to add this to other objects*** The interface is also seperate because everyone will have different implementations so having it implemented on the character would be best***

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UHealthComponent::LoseHealth(float Amount)
{
	Health -= Amount;

	if (GetOwner()->Implements<UHealthInterface>()) //Reason for spltting this into an interface is beacuse anyone who derives from HealthComponent.cpp may have different reactions and Object Check
	{
		//To call the interface you would call the derived interface and add Execute_ to the normal name. 1st parameter will always be GetOwner() is the implementer
		IHealthInterface::Execute_OnTakeDamage(GetOwner());
	}

	if (Health <= 0.f)
	{
		Health = 0.f;
		
		if (GetOwner()->Implements<UHealthInterface>())
		{
			IHealthInterface::Execute_OnDeath(GetOwner());
		}
	}
}

float UHealthComponent::GetHealthPercent() const
{
	return Health/100.f;
}