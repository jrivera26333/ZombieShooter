// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class ZOMBIESHOOTER_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	//Component for basic projectile movement
	class UProjectileMovementComponent* GetProjectileMovementComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Damage)
	float Damage = 34.f;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* DamageSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundAttenuation* BounceSoundAttenuation;

	UPROPERTY(EditAnywhere, Category = Particles)
	class UParticleSystem* HitParticles;

public:	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dodgeball, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dodgeball, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
};
