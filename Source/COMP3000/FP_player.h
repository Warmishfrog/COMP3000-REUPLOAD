#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "FPSProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "HandMesh.h"
#include "TimerManager.h"
#include "UpgradeComponent.h"
#include "Sound/SoundCue.h"
#include "FP_player.generated.h"


UCLASS()
class COMP3000_API AFP_player : public ACharacter
{
	GENERATED_BODY()	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "POVCamera")
	TSubclassOf<class AHandMesh> HandClass; // Class of the gun mesh to attach

	FTimerHandle  FireRateTimerHandle;
	FTimerHandle  AutoFireRateTimerHandle;


public:	

	// Sets default values for this character's properties
	AFP_player();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Camera

		//  mesh instance
		UPROPERTY(EditDefaultsOnly, Category = "POVCamera")
		AHandMesh* POVMesh;

		//  mesh offset and rotation
		UPROPERTY(EditDefaultsOnly, Category = "POVCamera")
		FVector POVMeshOffset;
		UPROPERTY(EditDefaultsOnly, Category = "POVCamera")
		FRotator POVMeshRotation;


	//Movement
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

	//gameplay
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Seconds;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Minutes;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bChoiceMade;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FOV = 90.0f;

	//Audio
		UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundCue* ShootingSFX;
		UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundCue* MovingSFX;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		float SFXVolume = 1.0f;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		float MusicVolume = 1.0f;


	//shooting
	
		// Function that handles firing projectiles.
		UFUNCTION()
		void Fire();

		UFUNCTION()
		void ResetCanFire();

		UFUNCTION()
		void StartFiring();

		UFUNCTION()
		void StopFiring();

		// Gun muzzle offset from the camera location.
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

		// Projectile class to spawn.
		UPROPERTY(EditDefaultsOnly, Category = Projectile)
		AFPSProjectile* Projectile;

		bool CanFire = true;
		UPROPERTY(EditAnywhere, Category = Projectile, meta = (ClampMin = "0.001"))
			float FireRate = 0.4f;

		bool bIsFiring= false;
	

	//health
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float  CurrentHealth = 0;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float  MaxHealth = 100;

		float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DEATH(); //for blueprint //ignore

	//XP
		UFUNCTION(BlueprintCallable, Category = "XP")
		void GainXP(float XPAmount);

		UFUNCTION(BlueprintCallable, Category = "XP")
		void LevelUp();

		UFUNCTION(BlueprintCallable, Category = "XP", BlueprintImplementableEvent)
		void LevelUpTRIGGER(); //for blueprint //ignore

		UPROPERTY(BlueprintReadWrite, Category = "XP")
			float XP = 0.0f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
			int XPToLevel = 0.0f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
			int Level = 0;

		UFUNCTION(BlueprintCallable, Category = "XP")
		void ApplyUpgradeEffect();
			
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UUpgradeComponent* PlayerUpgradeComponent;
		

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> CurrentThreeUpgrades;


		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FUpgradeData TempRow;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ChosenUpgrade = 0;

	//Projectile Variables

		//damage variables
		
		UPROPERTY(EditAnywhere, Category = Projectile, meta = (ClampMin = "0.1"))
		float val_Damage = 10.0f;

		UPROPERTY(EditAnywhere, Category = Projectile, meta = (ClampMin = "10.0"))
		float val_InitialSpeed = 1000.0f;

		UPROPERTY(EditAnywhere, Category = Projectile, meta = (ClampMin = "10.0"))
		float val_MaxSpeed = 3000.0f;

		UPROPERTY(EditAnywhere, Category = Projectile)
		bool val_bShouldBounce = false;

		UPROPERTY(EditAnywhere, Category = Projectile)
		float val_Bounciness = 0.3f;

		UPROPERTY(EditAnywhere, Category = Projectile)
		bool val_bRotationFollowsVelocity = true;

		UPROPERTY(EditAnywhere, Category = Projectile)
		float val_ProjectileGravityScale = 0.1f;

		UPROPERTY(EditAnywhere, Category = Projectile, meta = (ClampMin = "0.1"))

		float val_lifespan = 1.0f;

		UPROPERTY(EditAnywhere, Category = Projectile, meta = (ClampMin = "0.001"))
		float val_ProjectileScale = 0.09f;
		
		UPROPERTY(EditAnywhere, Category = Projectile)
		float val_CollisionSphere= 15.0f;

		UPROPERTY(EditAnywhere, Category = Projectile)
		float val_ExplosionRadius = 0.0f;

		UPROPERTY(EditAnywhere, Category = Projectile)
		int val_PierceCount = 0;

	//Upgrade Variables
		float XPModifier = 1.0f;
		float MovementSpeedModifier = 1.0f;
};
