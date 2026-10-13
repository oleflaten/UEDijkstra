// OEF 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DijPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ADijSpawner;

UCLASS()
class UEDIJKSTRA_API ADijPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LeftClickAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RightClickAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MiddleClickAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> StartAction;

	UPROPERTY()
	TObjectPtr<ADijSpawner> Spawner;

private:
	void OnLeftClick(const FInputActionValue& Value);
	void OnRightClick(const FInputActionValue& Value);
	void OnMiddleClick(const FInputActionValue& Value);
	void OnStartAction(const FInputActionValue& Value);
};