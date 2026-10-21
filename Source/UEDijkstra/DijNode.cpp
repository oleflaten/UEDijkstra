// OEF 2026

#include "DijNode.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"

ADijNode::ADijNode()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up the static mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(CubeMesh.Object);
	}

	LabelComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LabelComponent"));
	LabelComponent->SetupAttachment(MeshComponent);

	// Engine cube is 100 units tall and centered on its pivot, so the top face is at Z = 50
	LabelComponent->SetRelativeLocation(FVector(30.0f, 40.0f, 51.0f));
	LabelComponent->SetHorizontalAlignment(EHTA_Right);
	LabelComponent->SetVerticalAlignment(EVRTA_TextCenter);
	LabelComponent->SetWorldRotation(FRotator(90.0f, 0.0f, 180.0f));
	LabelComponent->SetWorldSize(30.0f);
	LabelComponent->SetTextRenderColor(FColor::Black);
	LabelComponent->SetText(FText::AsNumber(Cost));

	// The heuristic label is only used in A* mode. CreateDefaultSubobject can only be called in the constructor,
	// and the constructor cannot know how the node will be used, so the label is always created
	// and hidden until A* mode is enabled with SetAStar()
	LabelHeuristicComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HeuristicsLabel"));
	LabelHeuristicComponent->SetupAttachment(MeshComponent);

	// Engine cube is 100 units tall and centered on its pivot, so the top face is at Z = 50
	// Placed on the opposite face from the cost label, so the two labels never overlap
	LabelHeuristicComponent->SetRelativeLocation(FVector(0.0f, 40.0f, 51.0f));
	LabelHeuristicComponent->SetHorizontalAlignment(EHTA_Right);
	LabelHeuristicComponent->SetVerticalAlignment(EVRTA_TextCenter);
	LabelHeuristicComponent->SetWorldRotation(FRotator(90.0f, 0.0f, 180.0f));
	LabelHeuristicComponent->SetWorldSize(30.0f);
	LabelHeuristicComponent->SetTextRenderColor(FColor::Red);
	LabelHeuristicComponent->SetText(FText::AsNumber(Heuristics));
	LabelHeuristicComponent->SetVisibility(bAStar);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseColorMaterial(TEXT("/Game/Materials/M_BaseColor.M_BaseColor"));
	if (BaseColorMaterial.Succeeded())
	{
		MeshComponent->SetMaterial(0, BaseColorMaterial.Object);
		//UE_LOG(LogTemp, Warning, TEXT("Constructor: M_BaseColor found and assigned"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Did not find BaseColor material."));
	}

}

void ADijNode::SetMaterialColor(const FLinearColor& NewColor)
{
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue(TEXT("Color"), NewColor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No dynamic material. Check that the mesh has a material in slot 0."));
	}
}

void ADijNode::SetCost(int NewCost)
{
	Cost = NewCost;
	if (LabelComponent)
	{
		LabelComponent->SetText(FText::AsNumber(Cost));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LabelComponent is null. Cannot update cost label."));
	}
}

void ADijNode::SetAStar(bool bEnabled)
{
	bAStar = bEnabled;
	if (LabelHeuristicComponent)
	{
		LabelHeuristicComponent->SetVisibility(bAStar);
	}
}

void ADijNode::SetHeuristics(int NewHeuristics)
{
	Heuristics = NewHeuristics;
	if (LabelHeuristicComponent)
	{
		LabelHeuristicComponent->SetText(FText::AsNumber(Heuristics));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LabelHeuristicComponent is null. Cannot update heuristic label."));
	}
}

void ADijNode::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInterface* CurrentMaterial = MeshComponent->GetMaterial(0);
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay: slot 0 is %s"), CurrentMaterial ? *CurrentMaterial->GetName() : TEXT("none"));

	// Create a dynamic material instance for the mesh component
	DynMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);

	// Set the initial tint color
	if (DynMaterial)
	{
		// "Color" must match the parameter name in M_BaseColor exactly
		DynMaterial->SetVectorParameterValue(TEXT("Color"), TintColor);
		//UE_LOG(LogTemp, Warning, TEXT("Material color is set."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No dynamic material. Check that the mesh has a material in slot 0."));
	}

	// The constructor ran before the spawner (or the editor) could set bAStar,
	// so make the heuristic label visibility match bAStar now
	if (LabelHeuristicComponent)
	{
		LabelHeuristicComponent->SetVisibility(bAStar);
	}
}

void ADijNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
