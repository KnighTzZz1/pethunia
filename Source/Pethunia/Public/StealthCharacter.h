// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "StealthCharacter.generated.h"

class UCameraComponent;
class UCharacterMovementComponent;
class UCurveFloat;
class AGun;
class USkeletalMeshComponent;
/**
 * 
 */
UCLASS()
class PETHUNIA_API AStealthCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Models")
		USkeletalMeshComponent* Arms;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Models")
		USkeletalMeshComponent* PlayerWeapon;
public:
	AStealthCharacter();
	virtual void Tick(float DeltaTime);
	
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void DashAbility() override;
	
	UFUNCTION(Server, reliable)
		void Server_DashAbility();

	virtual void CrouchStart() override;

	UFUNCTION(Server, reliable)
		void Server_CrouchStart();

	UFUNCTION(Server, reliable)
		void Server_CrouchStop();

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
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
		TMap<int,AGun*> inv;


	UFUNCTION()
		void EquipPrimary();
	UFUNCTION(Server, reliable)
		void Server_EquipPrimary();
	UFUNCTION(NetMulticast, reliable)
		void Multi_EquipPrimary();
	
	UFUNCTION()
		void EquipSecondary();
	UFUNCTION(Server, reliable)
		void Server_EquipSecondary();
	UFUNCTION(NetMulticast, reliable)
		void Multi_EquipSecondary();



	UFUNCTION(BlueprintImplementableEvent)
		void SetupAnims();
	UFUNCTION(BlueprintImplementableEvent)
		void ClearAnims();
	UFUNCTION(BlueprintImplementableEvent)
		void PutWeaponOnBack(AActor* Weapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
		AGun* ActiveWeapon;


	void UpdateSlidingSpeed();
	UFUNCTION(Server, reliable)
		void Server_UpdateSlidingSpeed(FVector force);

	UFUNCTION(Server, reliable)
		void Server_Interact(AActor* ActorToInteract);

	UFUNCTION(NetMulticast, reliable)
		void Multi_Interact(AActor* ActorToInteract);

	UFUNCTION(Server, reliable)
		void Server_DropWeapon();
	UFUNCTION(NetMulticast, reliable)
		void Multi_DropWeapon();

	UFUNCTION()
		void TryPickingUpWeapon(AGun* weapon);

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

	bool shouldUpdateAngle();
	float GetSlidingAngle();
	bool SlideIsOnCooldown;
	FTimerHandle SlideHandle;
	void SlideCooldownOff();

	// Guns
	FTimerHandle ReloadTime;
	FTimerHandle ShootHandle;

	void LMB_Released();
	void ChangeFireMode();

	bool CanShoot;
};