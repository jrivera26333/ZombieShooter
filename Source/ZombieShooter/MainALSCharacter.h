// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSCharacter.h"
#include "HealthInterface.h"
#include "MainALSCharacter.generated.h"

//#include "Collectables/Gun.h"
class AGun;

UCLASS()
class ZOMBIESHOOTER_API AMainALSCharacter : public AALSCharacter, public IHealthInterface
{
	GENERATED_BODY()
	
public:
	AMainALSCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void OnDeath_Implementation() override;
	virtual void OnTakeDamage_Implementation() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	void ForwardMovement(float AxisValue);
	void RightMovement(float AxisValue);
	void OnFireButtonPressed();
	void OnFireButtonReleased();
	void CreateGun();
	void SwitchGun();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsFireButtonHeldDown = false;

	/** Gun class to spawn from **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* GunSkeletalMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGun> OneHandedGunClass;

	UPROPERTY()
	AGun* OneHandedGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGun> TwoHandedGunClass;

	UPROPERTY()
	AGun* TwoHandedGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AGun* PrimaryGun;

	const FString ActiveGunSocketPos = "ActiveGunPos";
	const FString OneHandedNonActiveSocketPos = "OneHandedNonActiveSocketPos";
	const FString TwoHandedNonActiveSocketPos = "TwoHandedNonActiveSocketPos";
};