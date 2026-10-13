// OEF 2026


#include "DijPlayerController.h"
#include "DijNode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Engine/LocalPlayer.h"

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
}

void ADijPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(ClickAction, ETriggerEvent::Started, this, &ADijPlayerController::OnLeftClick);
	}
}

void ADijPlayerController::OnLeftClick(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Left click detected"));

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit) && Hit.bBlockingHit)
	{
		if (ADijNode* ClickedNode = Cast<ADijNode>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Clicked node: %s, component: %s"),
				*GetNameSafe(ClickedNode), *GetNameSafe(Hit.GetComponent()));
		}
	}
}