// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainALSCharacter.h"
#include "ShooterALSCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AShooterALSCharacter : public AALSCharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	AGun* GetPrimaryWeapon();

	virtual void OnDeath_Implementation() override;
	virtual void OnTakeDamage_Implementation() override;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
private:
	void OnFireButtonPressed();
	void OnFireButtonReleased();
	void OnReloadButtonPressed();

	void Shoot();
	void SwitchPrimaryGun();
	void CreateGun(TSubclassOf<AGun> GunClass, AGun*& GunActor);
	void SetPrimaryWeapon(AGun* NewPrimaryGun);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsFireButtonHeldDown = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AGun* PrimaryGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGun> PistolGunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGun> RifleGunClass;

	AGun* Pistol;
	AGun* Rifle;

	const FString ActiveGunSocketPos = "ActiveGunPos";
};
