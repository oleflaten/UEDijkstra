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
	LabelComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	LabelComponent->SetHorizontalAlignment(EHTA_Center);
	LabelComponent->SetVerticalAlignment(EVRTA_TextCenter);
	LabelComponent->SetWorldRotation(FRotator(90.0f, 0.0f, 180.0f));
	LabelComponent->SetWorldSize(30.0f);
	LabelComponent->SetTextRenderColor(FColor::Black);
	LabelComponent->SetText(FText::AsNumber(Cost));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseColorMaterial(TEXT("/Game/Materials/M_BaseColor.M_BaseColor"));
	if (BaseColorMaterial.Succeeded())
	{
		MeshComponent->SetMaterial(0, BaseColorMaterial.Object);
		UE_LOG(LogTemp, Warning, TEXT("Constructor: M_BaseColor found and assigned"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Did not find BaseColor material."));
	}

}

void ADijNode::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInterface* CurrentMaterial = MeshComponent->GetMaterial(0);
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: slot 0 is %s"), CurrentMaterial ? *CurrentMaterial->GetName() : TEXT("none"));

	// Create a dynamic material instance for the mesh component
	DynMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);

	// Set the initial tint color
	if (DynMaterial)
	{
		// "Color" must match the parameter name in M_BaseColor exactly
		DynMaterial->SetVectorParameterValue(TEXT("Color"), TintColor);
		UE_LOG(LogTemp, Warning, TEXT("Material color is set."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No dynamic material. Check that the mesh has a material in slot 0."));
	}
	
}

void ADijNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
