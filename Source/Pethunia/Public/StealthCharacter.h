// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "StealthCharacter.generated.h"

class UCameraComponent;
class UCharacterMovementComponent;
class UCurveFloat;
class AGun;
/**
 * 
 */
UCLASS()
class PETHUNIA_API AStealthCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AStealthCharacter();
	virtual void Tick(float DeltaTime);
	
	virtual void DashAbility() override;

	virtual void CrouchStart() override;
	virtual void CrouchStop() override;
	
	virtual void Power1Activate() override;
	virtual void Power1Deactivate() override;

	virtual void Interact(AActor* ActorToInteract) override;

	virtual void LMB() override;
	virtual void Reload() override;
	virtual void DropWeapon() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player", meta = (ClampMin = "1.0", ClampMax = "2.0"))
		float IncreasedEnergyMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "1.0",ClampMax = "10.0"))
		float PowerDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "0.0",ClampMax = "20"))
		float PowerCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float PowerCost;
	
	// Dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float DashStopTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float DashCooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float DashCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float DashSpeed;

	void SetDashCooldownOff();
	void StopMovement();

	// Sliding
	float VanillaGroundFriction;
	float VanillaDeceleration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float SlideGroundFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float SlideDeceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float SlideBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding")
		float SlideDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding")
		UCurveFloat* SlideCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TArray<AActor*> WeaponInventory;


	UFUNCTION()
		void AddWeaponToInventory(AGun* weapon);

private:

	FVector LastLocation;

	// Power
	FTimerHandle Power1Handle;
	FTimerHandle Power1CooldownHandle;
	bool bPower1IsOnCooldown;
	void PowerCooldownOff();

	// Dashing
	bool DashIsOnCooldown;
	FTimerHandle DashHandle;

	// Sliding
	void UpdateSlidingSpeed();
	bool shouldUpdateAngle();
	float GetSlidingAngle();
	bool SlideIsOnCooldown;
	FTimerHandle SlideHandle;
	void SlideCooldownOff();

	// Guns
	AGun* ActiveWeapon;
	FTimerHandle ReloadTime;
	void ReloadWeapon();
	bool isReloading;
};