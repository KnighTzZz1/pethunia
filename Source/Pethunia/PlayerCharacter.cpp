// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"




// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArm->TargetArmLength = 0;
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PlayerStaticMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Static Mesh"));
	PlayerStaticMesh->SetupAttachment(RootComponent);
	TurnRate = 30.f;
	PitchRate = 30.f;
	JumpHeight = 45.f;
	MaxStamina = 500.f;
	isSprinting = false;
	Stamina = MaxStamina;
	GetCharacterMovement()->MaxWalkSpeed = maxSpeed;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::MoveForward(float value)
{
	if (Controller && value != 0.f)
	{
		FRotator ControllerRotation = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0, ControllerRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	if (Controller && value != 0.0f)
	{
		FRotator ControllerRotation = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0,ControllerRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void APlayerCharacter::LookHorizontal(float value)
{
	AddControllerYawInput(value * TurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookVertical(float value)
{
	AddControllerPitchInput(value * PitchRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::PlayerJump()
{
	if (isOnGround() && Stamina > 0) 
	{
		APlayerCharacter::LaunchCharacter(FVector(0.f, 0.f, JumpHeight), false, true);
		Stamina -= 35.f;
		GetWorldTimerManager().SetTimer(StaminaRechargeTimer, this, &APlayerCharacter::regenerateStamina, 2.0f, false);
	}
}

void APlayerCharacter::PlayerCrouch()
{
	Crouch();
}

void APlayerCharacter::PlayerProne()
{
	UnCrouch();
}

bool APlayerCharacter::isOnGround()
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	
	GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	FVector StartPosition = ViewPointLocation + FVector(0.f, 0.f, -88.f);
	FVector EndPosition = StartPosition + FVector(0.0f, 0.0f, -160.f);
	
	bool isHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPosition, EndPosition, ECC_Visibility, TraceParams);
	return isHit;
}

void APlayerCharacter::regenerateStamina()
{
	regStamina = true;
}

void APlayerCharacter::PlayerSprint()
{
	if (Stamina > 0) {
		GetCharacterMovement()->MaxWalkSpeed = maxSpeed * 1.5;
		isSprinting = true;
		regStamina = false;
	}
}

void APlayerCharacter::PlayerStopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = maxSpeed;
	isSprinting = false;
	GetWorldTimerManager().SetTimer(StaminaRechargeTimer, this, &APlayerCharacter::regenerateStamina, 2.0f, false);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp,Warning,TEXT("%f"),Stamina);
	if (isSprinting && Stamina > 0)
	{
		Stamina -= 1.f;
		if(Stamina <= 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = maxSpeed;
		}
	}
	if (!isSprinting && Stamina < MaxStamina && regStamina)
	{
		Stamina += 0.5f;
		if (Stamina >= MaxStamina)
		{
			regStamina = false;
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::PlayerJump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::PlayerCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::PlayerSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::PlayerStopSprint);
	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &APlayerCharacter::PlayerProne);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookVertical", this, &APawn::AddControllerPitchInput);
}


