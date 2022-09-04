// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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

bool AGun::DoesHaveEnoughAmmo()
{
	//We have ammo in the chamber to fire
	if(CurrentAmmoInMagazine > 0)
	{
		return true;
	}

	//We don't have anymore bullets
	return false;
}

bool AGun::ReloadGun()
{
	if (!IsClipEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Clip is not empty!"));
		return false;
	}


	//Max Amount that can be added
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

bool AGun::IsClipEmpty()
{
	if (CurrentAmmoInMagazine <= 0 && AmountOfAmmoOnReserve > 0)
		return true;
	else
		return false;
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

bool AGun::PullTrigger()
{
	if (SocketFirePointName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket Fire Gun Position is not set!"));
		return false;
	}

	if (!DoesHaveEnoughAmmo() || bIsOnCoolDown)
	{
		return false;
	}

	--CurrentAmmoInMagazine;
	StartCoolDownTimer();
	PlaySFX();

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);

		//if(OnParticleActorImpact)
		//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnParticleActorImpact, Hit.Location, ShotDirection.Rotation());

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);

			//if(CollisionSound)
			//	UGameplayStatics::PlaySoundAtLocation(this, CollisionSound, Hit.Location);
		}
	}

	return true;
}