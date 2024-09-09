// Fill out your copyright notice in the Description page of Project Settings.


#include "HandMesh.h"

AHandMesh::AHandMesh()
{
	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandMeshSceneComponent"));
	}

    if (!HandMeshComponent)
    {
		HandMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandMeshComponent"));
		HandMeshComponent->SetMobility(EComponentMobility::Movable);
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/hand.hand"));
        if (Mesh.Succeeded())
        {
			UE_LOG(LogTemp, Warning, TEXT("HandMesh found"));
			HandMeshComponent->SetStaticMesh(Mesh.Object);
			HandMeshComponent->SetMobility(EComponentMobility::Movable);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HandMesh not found"));
		}
		/*
		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Game/HandMaterial.HandMaterial"));
        if (Material.Succeeded())
        {
			HandMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, HandMeshComponent);
		}
		HandMeshComponent->SetMaterial(0, HandMaterialInstance);
		/**/
		//HandMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f)); //visual size of hand mesh
		HandMeshComponent->SetupAttachment(RootComponent);
		HandMeshComponent->SetMobility(EComponentMobility::Movable);
	}
}
