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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Bullets in Magazine **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int AmountOfAmmoOnReserve = 60;

	/** Bullets in Magazine **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MagazineSize = 15;

	/** Current ammo in magazine **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int CurrentAmmoInMagazine;

	/** Current ammo in magazine **/
	UPROPERTY(EditAnywhere, Category = "Gun Values")
		float FireCoolDown = 1;

	UPROPERTY(EditAnywhere, Category = "Gun Values")
		int MaxRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Gun Values")
		int Damage = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Gun Values")
		bool bIsAutomatic = false;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* MuzzleSound;

	/** Gun class to spawn from **/
	UPROPERTY(EditAnywhere, Category = "SFX")
		UParticleSystem* DefaultParticleImpact;

	/** Gun class to spawn from **/
	UPROPERTY(EditAnywhere, Category = "SFX")
		UParticleSystem* OnParticleGunFire;

	/** Gun class to spawn from **/
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, Category = "SFX")
		FString SocketFirePointName = "";

	/** Current ammo in magazine **/
	UPROPERTY(EditAnywhere)
	FString GunCharacterSocketName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool PullTrigger();
	bool ReloadGun();
	bool IsClipEmpty();
	bool IsAutomatic();

	FString GetGunCharacterSocketName();

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;

	FTimerHandle FiringCooldownHandle;

	void PlaySFX();
	void ResetCoolDown();
	void StartCoolDownTimer();
	bool bIsOnCoolDown = false;
	bool DoesHaveEnoughAmmo();
	//void DoesHaveEnoughAmmo();
};
