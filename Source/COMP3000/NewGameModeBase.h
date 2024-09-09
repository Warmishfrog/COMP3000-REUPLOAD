#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NewGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API ANewGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	ANewGameModeBase();

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		TSubclassOf<class ABasicEnemy> EnemyToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		bool CombatStarted = false;


	void SpawnEnemy();
	float SpawningRate(float t);

	//spawn parameters
		UPROPERTY(EditAnywhere, Category = "Spawn")
		float CumulativeSpawnTime = 0.0f;  // Cumulative time

		UPROPERTY(EditAnywhere, Category = "Spawn")
		float MaxSpawnRate = 70.0f;  // Maximum spawning rate

		UPROPERTY(EditAnywhere, Category = "Spawn")
		float GrowthRate = 0.6f;    // Growth rate parameter

		UPROPERTY(EditAnywhere, Category = "Spawn")
		float SpawnAccelerate = 4.0f;   // Time at which the spawning rate starts to accelerate

		UPROPERTY(EditAnywhere, Category = "Spawn")
		float BaseSpawnRate = 10.0f;   // Base spawning rate

		UPROPERTY(EditAnywhere, Category = "Spawn")
		int NumSpawnedEnemies = 0;   // Number of spawned enemies

		UPROPERTY(EditAnywhere, Category = "Spawn")
		int MaxSpawnedEnemies = 25;   // Maximum number of enemies that can be spawned

};
