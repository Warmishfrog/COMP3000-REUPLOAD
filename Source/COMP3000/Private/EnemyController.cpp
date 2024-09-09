// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyController::AEnemyController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));

	RootBox->SetGenerateOverlapEvents(true);

	RootBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyController::OnOverlap);
}

// Called when the game starts or when spawned
void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyController::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* collidedWith = (ACharacter*)OtherActor;

	if (collidedWith == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Detected"));
	}
}

