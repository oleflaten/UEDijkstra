// OEF 2026

#include "DijPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "DijNode.h"

ADijPlayer::ADijPlayer()
{
    PrimaryActorTick.bCanEverTick = false;

    // Take control of the player automatically when the level starts
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    // The camera follows the controller's look direction
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;

    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootScene);

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(RootScene);
}

void ADijPlayer::BeginPlay()
{
    Super::BeginPlay();

}

void ADijPlayer::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    UE_LOG(LogTemp, Warning, TEXT("Possessed by controller"));

    //if (APlayerController* PC = Cast<APlayerController>(NewController))
    //{
    //    FInputModeGameAndUI InputMode;
    //    InputMode.SetHideCursorDuringCapture(false);
    //    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    //    PC->SetInputMode(InputMode);
    //    PC->bShowMouseCursor = true;
    //}
}


void ADijPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    UE_LOG(LogTemp, Warning, TEXT("InputComponent called"));
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("LeftClick"), IE_Pressed, this, &ADijPlayer::OnLeftClick);

}


void ADijPlayer::OnLeftClick()
{
    UE_LOG(LogTemp, Log, TEXT("Left Click"));

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("No player controller found"));
        return;
    }

    FHitResult Hit;
    if (PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit) && Hit.bBlockingHit)
    {
        // The component that was hit, e.g. the static mesh or the text render
        UPrimitiveComponent* HitComponent = Hit.GetComponent();

        // The actor that owns it, cast to your node class
        ADijNode* ClickedNode = Cast<ADijNode>(Hit.GetActor());
        if (ClickedNode)
        {
            UE_LOG(LogTemp, Warning, TEXT("Clicked node: %s, component: %s"),
                *GetNameSafe(ClickedNode), *GetNameSafe(HitComponent));
        }
    }
}

/*
void ADijPlayer::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ADijPlayer::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}
*/
