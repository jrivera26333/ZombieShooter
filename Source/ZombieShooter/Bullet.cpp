// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetSphereRadius(35.f);

	//TODO: Configure Collision Matrix
	SphereComponent->SetCollisionProfileName(FName("Bullet"));
	SphereComponent->SetSimulatePhysics(true);

	//Simulation generates Hit Events
	SphereComponent->SetNotifyRigidBodyCollision(false);
	SphereComponent->SetGenerateOverlapEvents(true);

	RootComponent = SphereComponent;
	UE_LOG(LogTemp, Warning, TEXT("Created!"));

	//ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	//ProjectileMovement->InitialSpeed = 1500.f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//Adding our OnHit() to the bullet collision
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	
	UE_LOG(LogTemp, Warning, TEXT("Subscribed!"));
	//TODO: We will be doing a line trace
	//SetLifeSpan(5.f);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FORCEINLINE UProjectileMovementComponent * ABullet::GetProjectileMovementComponent() const
{
	return ProjectileMovement;
}

void ABullet::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Muzzle Sound
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 1.0f, 1.0f, 0.f, BounceSoundAttenuation);
	}

	//Muzzle Particles
	if (HitParticles != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorTransform());
	}

	//TODO: Alter for Enemy Character
	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	if (Player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit!"));

		//Instead of the Player handling health we are adding that as a component so it can be added to anyone else reason we are using UHealthComponent
		UHealthComponent* HealthComponent = Player->FindComponentByClass<UHealthComponent>();

		if (HealthComponent != nullptr)
		{
			HealthComponent->LoseHealth(Damage);
		}

		if (DamageSound != nullptr)
		{
			//TODO: Play Sound at Location
			UGameplayStatics::PlaySound2D(this, DamageSound);
		}

		Destroy();
	}
}