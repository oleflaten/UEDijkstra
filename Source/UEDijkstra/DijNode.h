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
	int Cost{ INT_MAX };

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	FLinearColor TintColor = FLinearColor::Blue;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial{ nullptr };
	// TObjectPtr<UMaterialInstanceDynamic> DynMaterial; // Modern Unreal Engine 5 way to declare a pointer
};

