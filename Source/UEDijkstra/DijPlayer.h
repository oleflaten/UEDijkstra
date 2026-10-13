// OEF 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DijPlayer.generated.h"

class UCameraComponent;
class USceneComponent;

UCLASS()
class UEDIJKSTRA_API ADijPlayer : public APawn
{
    GENERATED_BODY()

public:
    ADijPlayer();

protected:
    virtual void BeginPlay() override;

private:
	void OnLeftClick();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USceneComponent> RootScene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FirstPersonCamera;
};
