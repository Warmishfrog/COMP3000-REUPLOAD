// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSProjectile.generated.h"

UCLASS()
class COMP3000_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AFPSProjectile();

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Damage = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float CollisionRadius = 15.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileScale = 0.09f;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float InitialSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float MaxSpeed = 3000.0f;
	
	
	UPROPERTY (EditAnywhere, Category = "Projectile")
	float LifeSpan = 1.0f;
	/**/
	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool bShouldBounce = true;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Bounciness = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ExplosionRadius = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	int PierceCount = 0;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere collision component.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Projectile mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;

	// Projectile material
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	// Function that initializes the projectile's velocity in the shoot direction.
    void FireInDirection(const FVector& ShootDirection);

	// Function that is called when the projectile hits something.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
