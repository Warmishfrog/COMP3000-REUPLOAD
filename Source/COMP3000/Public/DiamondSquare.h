// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "DiamondSquare.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class COMP3000_API ADiamondSquare : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiamondSquare();
	//Trongle Sizes
	UPROPERTY(EditAnywhere, Meta = (CLampMin= 0 ))
		int Size = 2;

	//Perlin Noise modifiers
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
		float ZMultiplier = 500.0f;
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
		float NoiseScale = 0.1f;
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
		float XOffset = 0.0f;
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0))
		float YOffset = 0.0f;

	//Material and Vertices Scaling
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0.000001))
		float VertexDistance = 100.0f;
	UPROPERTY(EditAnywhere, Meta = (CLampMin = 0.000001))
		float UVScale = 1.0f;

	
		
	UFUNCTION(BlueprintCallable, Category = "Terrain Generation")
	void SetChunkParameters(int32 InSize, float InZMultiplier, float InNoiseScale, float InVertexDistance, float InUVScale, float InXOffset, float InYOffset);

	
	UFUNCTION(BlueprintCallable, Category = "Terrain Generation")
	void SetMaterial(UMaterialInterface* Material);
	
	//UPROPERTY(EditAnywhere, Category = "Material")
	//UMaterialInterface* Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UProceduralMeshComponent* ProceduralMesh;

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;

	void CreateVertices();
	void CreateTriangles();

};
