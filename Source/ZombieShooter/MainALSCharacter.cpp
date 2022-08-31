// Fill out your copyright notice in the Description page of Project Settings.


#include "MainALSCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Collectables/Gun.h"
#include "GameFramework/Character.h"

AMainALSCharacter::AMainALSCharacter(const FObjectInitializer& ObjectInitializer) : AALSCharacter(ObjectInitializer)
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

	UE_LOG(LogTemp, Warning, TEXT("Created Camera"));
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
}

void AMainALSCharacter::CreateGun()
{
	//Only to be created when in Rifle Mode. Note other State will need an offset for the Rifle
	if(GunSkeletalMeshClass && GetOverlayState() == EALSOverlayState::Rifle)
		AttachToHand(nullptr, GunSkeletalMeshClass, nullptr, false, FVector::ZeroVector);
}

void AMainALSCharacter::OnDeath_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Death!!"));
}

void AMainALSCharacter::OnTakeDamage_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Took Damage!!"));
}

// Called to bind functionality to input
void AMainALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMainALSCharacter::OnFireButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMainALSCharacter::OnFireButtonReleased);
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
	//Animation handles in BP_Anim
	UE_LOG(LogTemp, Warning, TEXT("Fire pew pew"));
	bIsFireButtonHeldDown = true;
}

void AMainALSCharacter::OnFireButtonReleased()
{
	//Animation handles in BP_Anim
	UE_LOG(LogTemp, Warning, TEXT("Fire button released"));
	bIsFireButtonHeldDown = false;
}