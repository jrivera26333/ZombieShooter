// Fill out your copyright notice in the Description page of Project Settings.


#include "MainALSCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Collectables/Gun.h"
#include "GameFramework/Character.h"

AMainALSCharacter::AMainALSCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	//We want to use the PawnRotation to rotate the spring arm
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritYaw = true;
	CameraBoom->bInheritRoll = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AMainALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Shoot();
}

void AMainALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetOverlayState(EALSOverlayState::Rifle);
	PlayerAnim = GetMesh()->GetAnimInstance();

	CreateGun(PistolGunClass, Pistol);
	CreateGun(RifleGunClass, Rifle);

	SetPrimaryWeapon(Pistol);
}

void AMainALSCharacter::SwitchPrimaryGun()
{
	if (PrimaryGun == Pistol)
		SetPrimaryWeapon(Rifle);
	else
		SetPrimaryWeapon(Pistol);
}

//Used for the Reload Montage. Thought: Every Gun could have a different montage
AGun* AMainALSCharacter::GetPrimaryWeapon()
{
	return PrimaryGun;
}

void AMainALSCharacter::CreateGun(TSubclassOf<AGun> GunClass, AGun*& GunActor)
{
	GunActor = GetWorld()->SpawnActor<AGun>(GunClass);
	GunActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(GunActor->GetGunCharacterSocketName()));
	GunActor->SetOwner(this);
}

void AMainALSCharacter::SetPrimaryWeapon(AGun* NewPrimaryGun)
{
	if (PrimaryGun != nullptr)
	{
		auto CachedPrimaryGun = PrimaryGun;
		CachedPrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(CachedPrimaryGun->GetGunCharacterSocketName()));
	}

	PrimaryGun = NewPrimaryGun;
	PrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(ActiveGunSocketPos));
}

// Called to bind functionality to input
void AMainALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMainALSCharacter::OnFireButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMainALSCharacter::OnFireButtonReleased);
	PlayerInputComponent->BindAction(TEXT("SwapWeaponAction"), IE_Pressed, this, &AMainALSCharacter::SwitchPrimaryGun);
	PlayerInputComponent->BindAction(TEXT("ReloadAction"), IE_Released, this, &AMainALSCharacter::OnReloadButtonPressed);
}

void AMainALSCharacter::ForwardMovement(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AMainALSCharacter::RightMovement(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AMainALSCharacter::Shoot()
{
	if (!bIsFireButtonHeldDown) return;

	if (PrimaryGun && !PrimaryGun->IsAutomatic())
	{
		bIsFireButtonHeldDown = false; //We only allow one shot
	}

	PrimaryGun->PullTrigger();
}

void AMainALSCharacter::OnFireButtonPressed()
{
	bIsFireButtonHeldDown = true;
}

void AMainALSCharacter::OnFireButtonReleased()
{
	bIsFireButtonHeldDown = false;
}

void AMainALSCharacter::OnReloadButtonPressed()
{
	PrimaryGun->PlayReloadMontageGun();
}

void AMainALSCharacter::OnDeath_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Death!!"));
}

void AMainALSCharacter::OnTakeDamage_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Took Damage!!"));
}