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
			// In A* mode the nodes also show their heuristic as a red label
			if (Node && bAStar)
			{
				Node->SetAStar(true);
			}
			// Even a failed spawn adds a nullptr, so the array index always matches the grid position
			Nodes.Add(Node);
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
	StopDijkstra();

	// We need a start node and an end node, and neither may be a wall
	if (!StartNode || !EndNode || StartNode->Cost == -1 || EndNode->Cost == -1)
	{
		ResetNodes();
		UE_LOG(LogTemp, Warning, TEXT("Cannot run Dijkstra. Need a start node (left click) and an end node (right click), and neither may be a wall."));
		return;
	}

	// Make sure the heuristic labels are up to date (in A* mode)
	UpdateHeuristics();

	InitializeDijkstra();

	// Start the timer to call StepDijkstra at intervals defined by StepDelay
	GetWorldTimerManager().SetTimer(StepTimerHandle, this, &ADijSpawner::StepDijkstra, StepDelay, true);
}

void ADijSpawner::UpdateHeuristics()
{
	// The heuristic label is only shown in A* mode
	if (!bAStar)
	{
		return;
	}

	// The heuristic needs both a start node and an end node
	const bool bValid = (StartNode && EndNode);

	int32 EndRow = 0;
	int32 EndColumn = 0;
	if (bValid)
	{
		GetGridCoordinates(EndNode, EndRow, EndColumn);
	}

	for (ADijNode* Node : Nodes)
	{
		if (!Node)
		{
			continue;
		}

		int32 Heuristics = 0;
		if (bValid)
		{
			int32 Row = 0;
			int32 Column = 0;
			GetGridCoordinates(Node, Row, Column);

			// Manhattan distance to the end node (the search only moves horizontal and vertical)
			Heuristics = FMath::Abs(Row - EndRow) + FMath::Abs(Column - EndColumn);
		}

		Node->SetHeuristics(Heuristics); // also updates the (red) heuristic label
	}
}

void ADijSpawner::InitializeDijkstra()
{
	// Put all nodes back in their starting state (walls, start and end keep their special look)
	ResetNodes();

	// Every non-wall node is unvisited until the algorithm finalizes it
	UnvisitedNodes.Reset();
	UnvisitedNodes.Reserve(Nodes.Num());
	for (ADijNode* Node : Nodes)
	{
		if (Node && Node->Cost != -1) // -1 = wall
		{
			UnvisitedNodes.Add(Node);
		}
	}

	// The search starts at the start node (its cost was already set to 0 in ResetNodes)
	CurrentNode = StartNode;
	bIsRunning = true;
}

void ADijSpawner::StopDijkstra()
{
	bIsRunning = false;
	GetWorldTimerManager().ClearTimer(StepTimerHandle);
}

void ADijSpawner::StepDijkstra()
{
	// Safety: if the run has stopped or there is no node left to finalize, do nothing
	if (!bIsRunning || !CurrentNode || !UnvisitedNodes.Contains(CurrentNode))
	{
		StopDijkstra();
		return;
	}

	// Finalize this node: take it out of the unvisited set and mark it as visited
	UnvisitedNodes.Remove(CurrentNode);
	CurrentNode->bIsVisited = true;

	// If we have reached the end node, the search is complete
	if (CurrentNode == EndNode)
	{
		TracePathAndColor();
		UE_LOG(LogTemp, Warning, TEXT("Dijkstra finished! The end node was reached with a cost of %d."), CurrentNode->Cost);
		StopDijkstra();
		return;
	}

	// A visited node is blue (the start node keeps its green color)
	if (CurrentNode != StartNode)
	{
		CurrentNode->SetMaterialColor(FLinearColor::Blue);
	}

	// Relax all horizontal and vertical neighbors (no diagonals)
	RelaxNeighbors(CurrentNode);

	// Pick the next node: the unvisited node with the lowest cost
	CurrentNode = GetNextCurrentNode();

	// If no reachable node is left, the end node cannot be reached
	if (!CurrentNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dijkstra finished without finding a path to the end node."));
		StopDijkstra();
	}
}

void ADijSpawner::RelaxNeighbors(ADijNode* Node)
{
	int32 Row = 0;
	int32 Column = 0;
	GetGridCoordinates(Node, Row, Column);

	// The four orthogonal directions: up, down, left, right (no diagonals)
	static const int32 Deltas[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

	for (const auto& Delta : Deltas)
	{
		ADijNode* Neighbor = GetNodeAt(Row + Delta[0], Column + Delta[1]);
		if (!Neighbor)
		{
			continue; // outside the grid
		}

		// Walls cannot be traversed, and already visited nodes cannot be improved
		if (Neighbor->Cost == -1 || Neighbor->bIsVisited)
		{
			continue;
		}

		// Moving to the neighbor costs 1
		const int32 NewCost = Node->Cost + 1;
		if (NewCost < Neighbor->Cost)
		{
			Neighbor->SetCost(NewCost); // also updates the cost label
			Neighbor->PreviousX = Row;
			Neighbor->PreviousY = Column;
		}
	}
}

void ADijSpawner::ResetNodes()
{
	for (ADijNode* Node : Nodes)
	{
		if (!Node)
		{
			continue;
		}

		// Every node goes back to the unvisited state
		Node->bIsVisited = false;
		Node->PreviousX = -1;
		Node->PreviousY = -1;

		// Walls keep their black color and -1 cost, the start node is green with cost 0, the end node is red, everything else is white
		if (Node->Cost == -1) // wall
		{
			Node->SetMaterialColor(FLinearColor::Black);
		}
		else if (Node == StartNode)
		{
			Node->SetMaterialColor(FLinearColor::Green);
			Node->SetCost(0);
		}
		else if (Node == EndNode)
		{
			Node->SetMaterialColor(FLinearColor::Red);
			Node->SetCost(999); // infinity - not reached yet
		}
		else
		{
			Node->SetMaterialColor(FLinearColor::White);
			Node->SetCost(999); // infinity - not reached yet
		}
	}
}

void ADijSpawner::TracePathAndColor()
{
	// Walk backwards from the end node to the start node, following the previous-node coordinates
	ADijNode* Node = EndNode;

	// The step counter is a safeguard against a broken previous-node chain
	for (int32 Steps = 0; Node && Node != StartNode && Steps < Nodes.Num(); ++Steps)
	{
		// Color the node yellow - the end node keeps its red color and the start node keeps its green color
		if (Node != EndNode)
		{
			Node->SetMaterialColor(FLinearColor::Yellow);
		}

		// Jump to the previous node in the grid
		Node = GetNodeAt(Node->PreviousX, Node->PreviousY);
	}
}

ADijNode* ADijSpawner::GetNextCurrentNode()
{
	ADijNode* BestNode = nullptr;
	int32 BestIndex = 0;

	for (ADijNode* Node : UnvisitedNodes)
	{
		// A node that is still at the "infinity" cost (999) has not been reached yet, so it is not a candidate
		if (!Node || Node->Cost == 999)
		{
			continue;
		}

		// The index in the grid (spawned row-major) gives us a stable tie-break order
		const int32 NodeIndex = Nodes.IndexOfByKey(TObjectPtr<ADijNode>(Node));

		// The lowest cost wins; ties are broken by grid position for a predictable animation
		if (!BestNode ||
			Node->Cost < BestNode->Cost ||
			(Node->Cost == BestNode->Cost && NodeIndex < BestIndex))
		{
			BestNode = Node;
			BestIndex = NodeIndex;
		}
	}

	return BestNode;
}

ADijNode* ADijSpawner::GetNodeAt(int32 Row, int32 Column) const
{
	// Outside the grid
	if (Row < 0 || Row >= Rows || Column < 0 || Column >= Columns)
	{
		return nullptr;
	}

	// The nodes were spawned row-major, so the coordinates map directly to the array index
	const int32 Index = Row * Columns + Column;
	if (Index < 0 || Index >= Nodes.Num())
	{
		return nullptr;
	}

	return Nodes[Index];
}

void ADijSpawner::GetGridCoordinates(ADijNode* Node, int32& OutRow, int32& OutColumn) const
{
	OutRow = -1;
	OutColumn = -1;

	// The nodes were spawned row-major, so the array index maps directly to the coordinates
	const int32 Index = Nodes.IndexOfByKey(TObjectPtr<ADijNode>(Node));
	if (Index == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("Node %s does not belong to the grid."), *GetNameSafe(Node));
		return;
	}

	OutRow = Index / Columns;
	OutColumn = Index % Columns;
}

