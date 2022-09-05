// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterALSCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Collectables/Gun.h"
#include "GameFramework/Character.h"

void AShooterALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetOverlayState(EALSOverlayState::Rifle);
	CreateGun(PistolGunClass, Pistol);
	CreateGun(RifleGunClass, Rifle);

	SetPrimaryWeapon(Pistol);
}

void AShooterALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Shoot();
}

// Called to bind functionality to input
void AShooterALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShooterALSCharacter::OnFireButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AShooterALSCharacter::OnFireButtonReleased);
	PlayerInputComponent->BindAction(TEXT("SwapWeaponAction"), IE_Pressed, this, &AShooterALSCharacter::SwitchPrimaryGun);
	PlayerInputComponent->BindAction(TEXT("ReloadAction"), IE_Released, this, &AShooterALSCharacter::OnReloadButtonPressed);
}

void AShooterALSCharacter::SwitchPrimaryGun()
{
	if (PrimaryGun == Pistol)
		SetPrimaryWeapon(Rifle);
	else
		SetPrimaryWeapon(Pistol);
}

//Used for the Reload Montage. Thought: Every Gun could have a different montage
AGun* AShooterALSCharacter::GetPrimaryWeapon()
{
	return PrimaryGun;
}

void AShooterALSCharacter::CreateGun(TSubclassOf<AGun> GunClass, AGun*& GunActor)
{
	GunActor = GetWorld()->SpawnActor<AGun>(GunClass);
	GunActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(GunActor->GetGunCharacterSocketName()));
	GunActor->SetOwner(this);
}

void AShooterALSCharacter::SetPrimaryWeapon(AGun* NewPrimaryGun)
{
	if (PrimaryGun != nullptr)
	{
		auto CachedPrimaryGun = PrimaryGun;
		CachedPrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(CachedPrimaryGun->GetGunCharacterSocketName()));
	}

	PrimaryGun = NewPrimaryGun;
	PrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(ActiveGunSocketPos));
}

void AShooterALSCharacter::Shoot()
{
	if (!bIsFireButtonHeldDown) return;

	if (PrimaryGun && !PrimaryGun->IsAutomatic())
	{
		bIsFireButtonHeldDown = false; //We only allow one shot
	}

	PrimaryGun->PullTrigger();
}

void AShooterALSCharacter::OnFireButtonPressed()
{
	bIsFireButtonHeldDown = true;
}

void AShooterALSCharacter::OnFireButtonReleased()
{
	bIsFireButtonHeldDown = false;
}

void AShooterALSCharacter::OnReloadButtonPressed()
{
	PrimaryGun->PlayReloadMontageGun();
}

void AShooterALSCharacter::OnDeath_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Death!!"));
}

void AShooterALSCharacter::OnTakeDamage_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Took Damage!!"));
}