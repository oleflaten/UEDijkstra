// OEF 2026


#include "DijPlayerController.h"
#include "DijNode.h"
#include "DijSpawner.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

void ADijPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerController BeginPlay: %s"), *GetNameSafe(this));

	Spawner = Cast<ADijSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ADijSpawner::StaticClass()));
	if (!Spawner)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ADijSpawner found in the level"));
	}

}

void ADijPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(LeftClickAction, ETriggerEvent::Started, this, &ADijPlayerController::OnLeftClick);
		EIC->BindAction(RightClickAction, ETriggerEvent::Started, this, &ADijPlayerController::OnRightClick);
		EIC->BindAction(MiddleClickAction, ETriggerEvent::Started, this, &ADijPlayerController::OnMiddleClick);
		EIC->BindAction(StartAction, ETriggerEvent::Started, this, &ADijPlayerController::OnStartAction);
	}
}

// Set Start node
void ADijPlayerController::OnLeftClick(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("PlayerController Left click detected"));
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit) && Hit.bBlockingHit)
	{
		if (ADijNode* ClickedNode = Cast<ADijNode>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Clicked node: %s, component: %s"),
				*GetNameSafe(ClickedNode), *GetNameSafe(Hit.GetComponent()));

			Spawner->StartNode = ClickedNode;
			ClickedNode->bIsStartNode = true;
			ClickedNode->SetMaterialColor(FLinearColor::Green);
			ClickedNode->SetCost(0);

			// Now that (potentially) both start and end are set, show the heuristics
			Spawner->UpdateHeuristics();
		}
	}
}

// Set End node
void ADijPlayerController::OnRightClick(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("PlayerController Right click detected"));
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit) && Hit.bBlockingHit)
	{
		if (ADijNode* ClickedNode = Cast<ADijNode>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Clicked node: %s, component: %s"),
				*GetNameSafe(ClickedNode), *GetNameSafe(Hit.GetComponent()));
			Spawner->EndNode = ClickedNode;
			ClickedNode->bIsEndNode = true;
			ClickedNode->SetMaterialColor(FLinearColor::Red);

			// Now that (potentially) both start and end are set, show the heuristics
			Spawner->UpdateHeuristics();
		}
	}
}

// Set Wall nodes
void ADijPlayerController::OnMiddleClick(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("PlayerController Middle click detected"));
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit) && Hit.bBlockingHit)
	{
		if (ADijNode* ClickedNode = Cast<ADijNode>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Clicked node: %s, component: %s"),
				*GetNameSafe(ClickedNode), *GetNameSafe(Hit.GetComponent()));
			
			ClickedNode->SetMaterialColor(FLinearColor::Black);
			ClickedNode->SetCost(-1);	//this means this node is a wall and cannot be traversed
		}
	}
}

// Run Dijkstra's algorithm - default is space bar, but can be changed in the input mapping context
void ADijPlayerController::OnStartAction(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("PlayerController Start action detected"));
	if (Spawner)
	{
		Spawner->RunDijkstra();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Spawner found to run Dijkstra's algorithm"));
	}
}