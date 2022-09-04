// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../MainALSCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = GunMesh;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoInMagazine = MagazineSize;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;

	return OwnerPawn->GetController();
}

void AGun::ResetCoolDown()
{
	bIsOnCoolDown = false;
	UE_LOG(LogTemp, Warning, TEXT("Cooldown Reset!"));
}

void AGun::PlaySFX()
{
	if (OnParticleGunFire)
		UGameplayStatics::SpawnEmitterAttached(OnParticleGunFire, GunMesh, FName(SocketFirePointName));

	if (MuzzleSound)
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMesh, FName(SocketFirePointName));
}

void AGun::StartCoolDownTimer()
{
	//TODO: Change
	bIsOnCoolDown = true;
	GetWorldTimerManager().SetTimer(FiringCooldownHandle, this, &AGun::ResetCoolDown, 1, false, FireCoolDown);
}

bool AGun::ReloadGun()
{
	//if (!IsClipEmpty())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Clip is not empty!"));
	//	return false;
	//}


	//Max Amount that can be added

	//TODO: Play Reload Montage Only/Move Code to Notify Event

	MainALSCharacter = Cast<AMainALSCharacter>(GetOwner());
	if (MainALSCharacter->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Character"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("null"));

	int GoalAmountToAdd = MagazineSize - CurrentAmmoInMagazine;
	
	if (GoalAmountToAdd > AmountOfAmmoOnReserve)
	{
		CurrentAmmoInMagazine += AmountOfAmmoOnReserve;
		AmountOfAmmoOnReserve = 0;
	}
	else
	{
		CurrentAmmoInMagazine += GoalAmountToAdd;
		AmountOfAmmoOnReserve -= GoalAmountToAdd;
	}

	return true;
}

void AGun::CheckIfClipEmpty()
{
	if (CurrentAmmoInMagazine <= 0 && AmountOfAmmoOnReserve > 0)
	{
		ReloadGun();
	}
}

bool AGun::IsAutomatic()
{
	return bIsAutomatic;
}

FString AGun::GetGunCharacterSocketName()
{
	return GunCharacterSocketName;
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();

	if (OwnerController == nullptr)
		return false;

	FVector OutPlayerLocation;
	FQuat OutRotation;

	GunMesh->GetSocketWorldLocationAndRotation(FName(SocketFirePointName), OutPlayerLocation, OutRotation);
	
	FVector End = OutPlayerLocation + (OutRotation.GetUpVector() * MaxRange);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); //Ignore the Gun
	Params.AddIgnoredActor(GetOwner()); //Ignore the Owner

	DrawDebugLine(
		GetWorld(),
		OutPlayerLocation,
		End,
		FColor(255, 0, 0),
		false, -1, 0,
		12.333
	);

	return GetWorld()->LineTraceSingleByChannel(Hit, OutPlayerLocation, End, ECollisionChannel::ECC_Pawn, Params);
}

void AGun::PullTrigger()
{
	if (bIsOnCoolDown || CurrentAmmoInMagazine <= 0) return;		

	--CurrentAmmoInMagazine;
	StartCoolDownTimer();
	PlaySFX();

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}

	CheckIfClipEmpty();
}