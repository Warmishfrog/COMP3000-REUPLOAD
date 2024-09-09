// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeGen.h"
#include "ProceduralMeshComponent.h"
#include "DiamondSquare.h"
#include "Kismet/GameplayStatics.h"


ALandscapeGen::ALandscapeGen()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALandscapeGen::BeginPlay()
{
    Super::BeginPlay();

    // debug for beginplay
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ALandscapeGen BeginPlay"));
    ChunkFolderPath = FName(TEXT("Chunk Folder")); //debug for condensing actors

    RandomXOffset = FMath::RandRange(0.0f, 10000.0f); // Adjust the range as needed
    RandomYOffset = FMath::RandRange(0.0f, 10000.0f);
}

void ALandscapeGen::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Get the player location
    FVector PlayerLocation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation();

   // FVector SpawnLocation(Current.X, Current.Y, 0.0f); //check if this is needed

    Current.X = FMath::RoundToInt((PlayerLocation.X)/VertexDistance);
    Current.Y = FMath::RoundToInt((PlayerLocation.Y)/VertexDistance); 



    for (int X = -Distance; X <= Distance; X++)
    {
        for (int Y = -Distance; Y <= Distance; Y++)
        {            
            Visible.X = (Current.X + X);
            Visible.Y = (Current.Y + Y);
            bool GenVis = Generated.Contains(FChunkPosition(Visible)); // if it already exists
            
            if (!GenVis) 
            {
                FActorSpawnParameters SpawnParams;
                FVector Vector = FVector((Visible.X * VertexDistance), (Visible.Y * VertexDistance), 0);
                //UE_LOG(LogTemp, Warning, TEXT("Vector: %s"), *Vector.ToString());
                ADiamondSquare* DiamondSquareActor = GetWorld()->SpawnActor<ADiamondSquare>(ADiamondSquare::StaticClass(), Vector, FRotator::ZeroRotator, SpawnParams);

                /*
                DiamondSquareActor->SetFolderPath(ChunkFolderPath);
                // Set a name for the spawned actor
                FString ActorName = FString::Printf(TEXT("Chunk_%d (%d %d)"), ChunkCount, Visible.X, Visible.Y);
                DiamondSquareActor->SetActorLabel(*ActorName);
                /**/
                
                //Setting Parameters
                XOffset = Size * Visible.X + RandomXOffset;
                YOffset = Size * Visible.Y + RandomYOffset;
                DiamondSquareActor->SetChunkParameters(Size, ZMultiplier, NoiseScale, VertexDistance / Size, UVScale, XOffset, YOffset);
                DiamondSquareActor->SetMaterial(Material);
                    
                //DiamondSquareActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
                Generated.Add(FChunkPosition(Visible));
                ChunkCount++; //debug

            }            
        }
    }
}

