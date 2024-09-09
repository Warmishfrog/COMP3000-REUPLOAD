#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FindPlayerPosition.generated.h"

UCLASS()
class COMP3000_API AFindPlayerPosition : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFindPlayerPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};