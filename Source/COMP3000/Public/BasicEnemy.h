// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

UCLASS()
class COMP3000_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:	
	bool PlayerDetected;
	bool CanAttackPlayer;


	UPROPERTY(BlueprintReadWrite)
	bool CanDealDamage;

	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerCollisionDetection;

	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerAttackCollisionDetection;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageCollision;
	//
	class ABasicEnemyAIController* BasicEnemyAIController;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	UPROPERTY(EditAnywhere)
	float StoppingDistance = 100.0f;

	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;

	FTimerHandle SeekPlayerTimerHandle;

	UFUNCTION()
	void MoveToPlayer();

	UFUNCTION()
	void SeekPlayer();

	UFUNCTION()
	void StopSeekingPlayer();

	UFUNCTION()
	void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	//enemy health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float EnemyHealth = 40.0f;
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


};
