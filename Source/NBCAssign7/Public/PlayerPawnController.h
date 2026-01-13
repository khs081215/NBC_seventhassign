// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerPawnController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class NBCASSIGN7_API APlayerPawnController : public APlayerController
{
	GENERATED_BODY()
	APlayerPawnController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|PlayerPawn")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|PlayerPawn")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|PlayerPawn")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputMappingContext* DroneInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneRollAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneDownAction;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void togglestate();
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool GetisPlayerPawnState();


protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	bool isPlayerPawnState;

};
