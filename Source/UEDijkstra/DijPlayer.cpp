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
