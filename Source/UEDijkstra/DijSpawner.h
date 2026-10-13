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

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	int Rows = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	int Columns = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	float Spacing = 105.0f;

	// The node class to spawn. Can be set to a Blueprint subclass of ADijNode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADijNode> NodeClass = ADijNode::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	ADijNode* StartNode{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra", meta = (AllowPrivateAccess = "true"))
	ADijNode* EndNode{ nullptr };

	UPROPERTY()
	TArray<TObjectPtr<ADijNode>> Nodes;	

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComponent;	// A dummy just to be able to place the spawner in the world, even if it is empty
};
	