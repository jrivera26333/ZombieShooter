// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

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

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();

	if (OwnerController == nullptr)
		return false;

	FVector PlayerLocation;
	FRotator PlayerRotation;
	OwnerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	ShotDirection = -PlayerRotation.Vector(); //It will be facing us now

	FVector End = PlayerLocation + PlayerRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); //Ignore the Gun
	Params.AddIgnoredActor(GetOwner()); //Ignore the Owner

	return GetWorld()->LineTraceSingleByChannel(Hit, PlayerLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AGun::PullTrigger()
{
	if (SocketFirePointName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket Fire Gun Position is not set!"));
		return;
	}

	UGameplayStatics::SpawnEmitterAttached(OnParticleGunFire, GunMesh, FName(SocketFirePointName));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMesh, FName(SocketFirePointName));

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnParticleActorImpact, Hit.Location, ShotDirection.Rotation());

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			UGameplayStatics::PlaySoundAtLocation(this, CollisionSound, Hit.Location);
		}

	}
}