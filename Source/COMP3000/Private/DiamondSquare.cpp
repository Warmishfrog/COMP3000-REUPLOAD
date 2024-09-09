
#include "DiamondSquare.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

ADiamondSquare::ADiamondSquare()
{
	PrimaryActorTick.bCanEverTick = false;

	//ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	//ProceduralMesh->SetupAttachment(GetRootComponent());

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootComp;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMesh->SetupAttachment(RootComponent);
}

void ADiamondSquare::OnConstruction(const FTransform& Transform)
{	
	Super::OnConstruction(Transform);

	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();	
}

void ADiamondSquare::BeginPlay()
{

	Super::BeginPlay();
}


void ADiamondSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ADiamondSquare::CreateVertices() 
{
	for (int X = 0; X <= Size; ++X) 
	{
		for (int Y = 0; Y <= Size; ++Y)
		{
			float Z = FMath::PerlinNoise2D(FVector2D(NoiseScale * (X + 0.1 + XOffset), NoiseScale * (Y + 0.1 + YOffset))) * ZMultiplier;
			//GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z)); //debug for Z positions
			Vertices.Add(FVector((Size/-2 + X) * VertexDistance, (Size/-2 + Y) * VertexDistance, Z)); //X = (Size/2 +X)* VertexDistance //coordinates
			UV0.Add(FVector2D((Size / 2 + X) * UVScale, (Size / 2 + Y) * UVScale));

			//Debug to show vertices corners!!!!!
			//DrawDebugSphere(GetWorld(), FVector(X * Scale, Y * Scale, 0), 25.0f, 16, FColor::Red, true, -1.0f, 0U, 0.0f);
		}
	}
}
void ADiamondSquare::CreateTriangles()
{
	int Vertex = 0;

	for (int X = 0; X < Size; ++X)
	{
		for (int Y = 0; Y < Size; ++Y)
		{
			Triangles.Add(Vertex); //Bottom Left Corner
			Triangles.Add(Vertex + 1); //Bottom Right Corner
			Triangles.Add(Vertex + Size + 1); //Top Left Corner
			//Second Triangle
			Triangles.Add(Vertex + 1); //Bottom Right Corner
			Triangles.Add(Vertex + Size + 2); //Top Right Corner
			Triangles.Add(Vertex + Size + 1); //Top Left Corner

			++Vertex;
		}
		++Vertex;
	}
}

void ADiamondSquare::SetChunkParameters(int32 InSize, float InZMultiplier, float InNoiseScale, float InVertexDistance, float InUVScale, float InXOffset, float InYOffset)
{
	Size = InSize;
	ZMultiplier = InZMultiplier;
	NoiseScale = InNoiseScale;
	VertexDistance = InVertexDistance;
	UVScale = InUVScale;
	XOffset = InXOffset;
	YOffset = InYOffset;

	CreateVertices();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
}

void ADiamondSquare::SetMaterial(UMaterialInterface* Material)
{
	if (ProceduralMesh)
	{
		// Set the new material for the procedural mesh component
		ProceduralMesh->SetMaterial(0, Material);
	}
}

