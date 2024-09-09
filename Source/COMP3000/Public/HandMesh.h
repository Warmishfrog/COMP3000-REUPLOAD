// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "HandMesh.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API AHandMesh : public AStaticMeshActor
{
	GENERATED_BODY()
	
	public:
		AHandMesh();

		//  mesh
		UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* HandMeshComponent;

		//  material
		UPROPERTY(VisibleDefaultsOnly)
		UMaterialInstanceDynamic* HandMaterialInstance;


};
