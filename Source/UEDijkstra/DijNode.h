// OEF 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DijNode.generated.h"

UCLASS()
class UEDIJKSTRA_API ADijNode : public AActor
{
	GENERATED_BODY()
	
public:	
	ADijNode();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	bool bIsStartNode{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	bool bIsEndNode{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	bool bIsVisited{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	int Cost{ 999 };	// should be infinity at start, but that is too big for the label, so we use 999 instead - OK till 31x31
						// if this is set to -1 , it means this node is a wall and cannot be traversed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	int Heuristics{ 0 };

	void SetMaterialColor(const FLinearColor& NewColor);
	void SetCost(int NewCost);

	// Enables or disables A* mode: shows or hides the (red) heuristic label
	void SetAStar(bool bEnabled);

	void SetHeuristics(int NewHeuristics);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextRenderComponent> LabelComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextRenderComponent> LabelHeuristicComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	FLinearColor TintColor = FLinearColor::White;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial{ nullptr };
	// TObjectPtr<UMaterialInstanceDynamic> DynMaterial; // Modern Unreal Engine 5 way to declare a pointer

	// Previous cheapest node coordinates in the grid
	// Used for backtracking the path after Dijkstra's algorithm has completed
	int PreviousX{ -1 };
	int PreviousY{ -1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	bool bAStar = false;

	friend class ADijPlayerController; // Allow the player controller to access private members of this class
	friend class ADijSpawner; // Allow the spawner to access private members (the Dijkstra algorithm reads and writes PreviousX/PreviousY)
};



// Idea:
// The color will tell if it is visited
//     White = unvisited
//     Blue = visited
//	   Green = start node
//	   Red = end node
//	   Yellow = part of the final path
//     Black = wall   - cost = -1, you cannot traverse this node

