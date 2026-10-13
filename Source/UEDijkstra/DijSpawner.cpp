// OEF 2026

#include "DijSpawner.h"
#include "Engine/World.h"

ADijSpawner::ADijSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create a default static mesh component and set it as the root component
	// Even if it is empty, it will allow us to place the spawner in the world
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

}

void ADijSpawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World || !NodeClass)
	{
		return;
	}

	const FVector Origin = GetActorLocation();

	Nodes.Reserve(Rows * Columns);

	// Spawn a grid of nodes
	for (int32 Row = 0; Row < Rows; ++Row)
	{
		for (int32 Column = 0; Column < Columns; ++Column)
		{
			const FVector Location = Origin + FVector(Row * Spacing, Column * Spacing, 0.0f);

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			ADijNode* Node = World->SpawnActor<ADijNode>(NodeClass, Location, FRotator::ZeroRotator, SpawnParams);
			if (Node)
			{
				Nodes.Add(Node);
			}
		}
	}
}

void ADijSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADijSpawner::RunDijkstra()
{
	UE_LOG(LogTemp, Warning, TEXT("RunDijkstra called! Now things should happen..."));

	// Restart cleanly if a run is already in progress
	GetWorldTimerManager().ClearTimer(StepTimerHandle);

	// Start the timer to call StepDijkstra at intervals defined by StepDelay
	GetWorldTimerManager().SetTimer(StepTimerHandle, this, &ADijSpawner::StepDijkstra, StepDelay, true);
}

void ADijSpawner::StepDijkstra()
{
	UE_LOG(LogTemp, Warning, TEXT("Stepping Dijkstra! Things are happening..."));
}

