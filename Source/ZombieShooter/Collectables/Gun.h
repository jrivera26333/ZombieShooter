// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun(); 
	int GetCurrentAmmoInMagazine();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FireLineTrace();

	/** Bullets in Magazine **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int MagazineSize = 15;

	/** Max amount of clips **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int MaxAmountOfMagazines = 2;

	/** Current ammo in magazine **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int CurrentAmmoInMagazine;

	/** Current ammo in magazine **/
	UPROPERTY(EditAnywhere, Category = "Gun Values")
		int FireCoolDown = 1;

	UPROPERTY(EditAnywhere, Category = "Gun Values")
		int MaxRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Gun Values")
		int Damage = 1000.f;

	/** Gun class to spawn from **/
	UPROPERTY(EditAnywhere, Category = "SFX")
		UParticleSystem* OnParticleActorImpact;

	/** Gun class to spawn from **/
	UPROPERTY(EditAnywhere, Category = "SFX")
		UParticleSystem* OnParticleGunFire;

	/** Gun class to spawn from **/
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* GunMesh;

	//Going to use Sound Cues to alter the pitch later
	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
		USoundBase* CollisionSound;

	UPROPERTY(EditAnywhere, Category = "SFX")
		FString SocketFirePointName = "";

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PullTrigger();

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;

	FTimerHandle FiringCooldownHandle;

	void ResetCoolDown();
	void PlaySFX();
	void StartCoolDownTimer();
	bool bIsOnCoolDown = false;
};
