// OEF 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DijNode.h"
#include "DijSpawner.generated.h"

UCLASS()
class UEDIJKSTRA_API ADijSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	ADijSpawner();
	virtual void Tick(float DeltaTime) override;

	void RunDijkstra();

protected:
	virtual void BeginPlay() override;

private:
	void StepDijkstra();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	int Rows = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	int Columns = 15;

	// Cube is 100, so we add 5 to have a small gap between the cubes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	float Spacing = 105.0f;

	// The node class to spawn. Can be set to a Blueprint subclass of ADijNode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADijNode> NodeClass = ADijNode::StaticClass();

	// Pointer to the start node in the grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	ADijNode* StartNode{ nullptr };

	// Pointer to the end node in the grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	ADijNode* EndNode{ nullptr };

	// Array to hold references to all spawned nodes
	UPROPERTY()
	TArray<TObjectPtr<ADijNode>> Nodes;	

	// Delay between each step of the Dijkstra algorithm, in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true", ClampMin = "0.01"))
	float StepDelay = 0.3f;

	FTimerHandle StepTimerHandle;


	// A dummy just to be able to place the spawner in the world, even if it is empty
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComponent;	


	friend class ADijPlayerController;
};
	