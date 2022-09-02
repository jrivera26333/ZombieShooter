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
}

void AMainALSCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetOverlayState(EALSOverlayState::Rifle);
	CreateGun();
	PlayerAnim = GetMesh()->GetAnimInstance();
	PlayerAnim->OnMontageEnded.AddDynamic(this, &AMainALSCharacter::OnMontageFireComplete);
}

void AMainALSCharacter::CreateGun()
{
	if (!OneHandedGunClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("OneHandedGunClass not registered!"));
		return;
	}
	else
	{
		OneHandedGun = GetWorld()->SpawnActor<AGun>(OneHandedGunClass);
		OneHandedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(ActiveGunSocketPos));
		OneHandedGun->SetOwner(this);
	}

	PrimaryGun = OneHandedGun;

	if (!TwoHandedGunClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("OneHandedGunClass not registered!"));
		return;
	}
	else
	{
		TwoHandedGun = GetWorld()->SpawnActor<AGun>(TwoHandedGunClass);
		TwoHandedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TwoHandedNonActiveSocketPos));
		TwoHandedGun->SetOwner(this);
	}
}

void AMainALSCharacter::SwitchGun()
{
	if (OneHandedGun == nullptr|| TwoHandedGun == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("One of your guns is not registered"));
		return;
	}

	if (PrimaryGun == OneHandedGun)
	{
		OneHandedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(OneHandedNonActiveSocketPos));
		TwoHandedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(ActiveGunSocketPos));

		PrimaryGun = TwoHandedGun;
	}
	else
	{
		OneHandedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(ActiveGunSocketPos));
		TwoHandedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TwoHandedNonActiveSocketPos));

		PrimaryGun = OneHandedGun;
	}
}

// Called to bind functionality to input
void AMainALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMainALSCharacter::OnFireButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMainALSCharacter::OnFireButtonReleased);
	PlayerInputComponent->BindAction(TEXT("SwapWeaponAction"), IE_Released, this, &AMainALSCharacter::SwitchGun);
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

void AMainALSCharacter::OnFireButtonPressed()
{
	if (PrimaryGun && PrimaryGun->PullTrigger())
	{
		bIsFireButtonHeldDown = true;

		if (FireMontage)
		{
			PlayerAnim->Montage_Play(FireMontage);
		}
	}
}

void AMainALSCharacter::OnFireButtonReleased()
{
	//TODO: Setup Automatic Mode
	bIsFireButtonHeldDown = false;
}

void AMainALSCharacter::OnReloadButtonPressed()
{
	if (PrimaryGun && PrimaryGun->ReloadGun())
	{
		bIsFireButtonHeldDown = true;

		if(ReloadMontage)
		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
	}
}

void AMainALSCharacter::OnDeath_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Death!!"));
}

void AMainALSCharacter::OnTakeDamage_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Took Damage!!"));
}


void AMainALSCharacter::OnMontageFireComplete(UAnimMontage* GunFireMontage, bool isFinished)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire complete!"));
}