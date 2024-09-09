#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSProjectile.h"
#include "TimerManager.h"
#include "FP_player_cpp.generated.h"


UCLASS()
class COMP3000_API AFP_player_cpp : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

public:

	// Sets default values for this character's properties
	AFP_player_cpp();

	bool CanFire = true;
	float FireRate = 0.5f;
	bool bIsFiring;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();

	// Handles input for moving forward and backward.
	UFUNCTION()
	void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
	void MoveRight(float Value);

	// Sets jump flag when key is pressed.
	UFUNCTION()
	void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
	void StopJump();

	// Function that handles firing projectiles.
	UFUNCTION()
	void Fire();

	UFUNCTION()
	void ResetCanFire();

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	FTimerHandle  FireRateTimerHandle;
	FTimerHandle  AutoFireRateTimerHandle;
};
