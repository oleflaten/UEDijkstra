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
	}
}

void ADijPlayerController::OnLeftClick(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerController Left click detected"));

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
		}
	}
}

void ADijPlayerController::OnRightClick(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerController Right click detected"));
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
		}
	}
}