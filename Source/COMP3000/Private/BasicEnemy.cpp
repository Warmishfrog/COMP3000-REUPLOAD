
#include "BasicEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "BasicEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
//#include "AIController.h"
#include "AITypes.h"
#include "Engine/DamageEvents.h"
#include <COMP3000/FP_player.h>

// Sets default values
ABasicEnemy::ABasicEnemy()
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCollisionDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));
	PlayerCollisionDetection->SetupAttachment(RootComponent);

	PlayerAttackCollisionDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));
	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	
	AController* EnemyController = GetController();
	if (GetController() != nullptr)
	{
		// The enemy already has a controller
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Enemy already has a controller! REMAPPING"));
		BasicEnemyAIController = Cast<ABasicEnemyAIController>(EnemyController); //crashes without this line
	}
	else
	{
		// The enemy doesn't have a controller yet
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy doesn't have a controller! ASSIGNING"));
		ABasicEnemyAIController* Casted = GetWorld()->SpawnActor<ABasicEnemyAIController>(ABasicEnemyAIController::StaticClass());
		if (Casted)
		{
			BasicEnemyAIController = Casted;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Successfully spawned AI controller finally"));
		}
	}
	if (BasicEnemyAIController)
	{
		BasicEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABasicEnemy::OnAIMoveCompleted);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BasicEnemyAIController is NULL"));
	}
	/**/
	
	//Collision Component Setup
		PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
			&ABasicEnemy::OnPlayerDetectedOverlapBegin);
		PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
			&ABasicEnemy::OnPlayerAttackOverlapBegin);
		PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
			&ABasicEnemy::OnPlayerAttackOverlapEnd);
		DamageCollision->OnComponentBeginOverlap.AddDynamic(this,
			&ABasicEnemy::OnDealDamageOverlapBegin);

		InitialLifeSpan = 30.0f;
}

void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (PlayerDetected && CanAttackPlayer)
	{
		StopSeekingPlayer();

		// attack player
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Enemy is attacking"));
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->TakeDamage(10.0f, FDamageEvent(), BasicEnemyAIController, this);
	}
	if (!PlayerDetected)
	{
		Destroy();
	}
}

void ABasicEnemy::MoveToPlayer()
{
	FVector PlayerLocation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation();
	if (BasicEnemyAIController)
	{
		BasicEnemyAIController->MoveToLocation(PlayerLocation, StoppingDistance, true);
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("BasicEnemyAIController is NULL"));
	}
}

void ABasicEnemy::SeekPlayer()
{
	MoveToPlayer();
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this,
		&ABasicEnemy::SeekPlayer, 0.25f, true);
}

void ABasicEnemy::StopSeekingPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

void ABasicEnemy::OnPlayerDetectedOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("AI detected player")));

		PlayerDetected = true;
		SeekPlayer();
	}
}

void ABasicEnemy::OnPlayerDetectedOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		UE_LOG(LogTemp, Log, TEXT("Enemy left bounds"));
		PlayerDetected = false;
		StopSeekingPlayer();
		Destroy();

	}
}

void ABasicEnemy::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		CanAttackPlayer = true;
	}
}

void ABasicEnemy::OnPlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		CanAttackPlayer = false;
		SeekPlayer();
	}
}

void ABasicEnemy::OnDealDamageOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) && CanDealDamage)
	{
		// deal damage to player
	}
}

float ABasicEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageCaused = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageCaused = FMath::Min(EnemyHealth, DamageCaused);
	EnemyHealth -= DamageCaused;
	UE_LOG(LogTemp, Log, TEXT("Damage amount: %f"), DamageAmount);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Enemy is taking damage"));
	if (EnemyHealth <= 0)
	{
		// Award XP to the player
		if (EventInstigator && EventInstigator->GetPawn())
		{
			AFP_player* PlayerPawn = Cast<AFP_player>(EventInstigator->GetPawn());
			if (PlayerPawn)
			{
				PlayerPawn->GainXP(FMath::RandRange(1, 3)); //amount of XP gained
			}
		}
		Destroy();
	}
	return 0.0f;
}

