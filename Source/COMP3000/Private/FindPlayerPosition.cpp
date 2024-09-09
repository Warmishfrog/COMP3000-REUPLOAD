#include "FindPlayerPosition.h"

// Sets default values
AFindPlayerPosition::AFindPlayerPosition()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFindPlayerPosition::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFindPlayerPosition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController && PlayerController->GetPawn())
    {
        FVector MyCharacter = PlayerController->GetPawn()->GetActorLocation();
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Player Location: %s"), *MyCharacter.ToString()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player or Controller not found!"));
    }
}