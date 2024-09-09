// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "LandscapeGen.generated.h"

USTRUCT(BlueprintType)
struct FChunkPosition
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;


	//set values to 0 by default
	FChunkPosition()
		: X(0)
		, Y(0)
	{}	
	// Define the equality operator (==) so i can compare other FChunkPositions
	bool operator==(const FChunkPosition& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	// Define ValueTypeHash for FChunkPosition //needed for TSet and Tmap to work //pain in ass
	friend uint32 GetTypeHash(const FChunkPosition& ChunkPosition)
	{
		return HashCombine(GetTypeHash(ChunkPosition.X), GetTypeHash(ChunkPosition.Y));
	}
};

UCLASS()
class COMP3000_API ALandscapeGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandscapeGen();

	UPROPERTY(EditAnywhere)
	int Distance;

	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
	int Size = 1;

	//Perlin Noise modifiers
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
	float ZMultiplier = 500.0f;
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
	float NoiseScale = 0.1f;
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
	float XOffset = 0.0f;
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
	float YOffset = 0.0f;

	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
	float RandomXOffset;

	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
	float RandomYOffset;

	//Material and Vertices Scaling
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0.000001))
	float VertexDistance = 100.0f;
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0.000001))
	float UVScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Material")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Position")
	FChunkPosition Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Position")
	FChunkPosition Visible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Position")
	FName ChunkFolderPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Position")
	int ChunkCount;
	
	UPROPERTY(EditAnywhere, Category = "Chunk Position")
	TSet<FChunkPosition> Generated;



	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

};
