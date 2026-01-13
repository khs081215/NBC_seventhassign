// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnController.h"
#include "EnhancedInputSubsystems.h"


APlayerPawnController::APlayerPawnController() :
    InputMappingContext(nullptr),
    MoveAction(nullptr),
    LookAction(nullptr),
    DroneInputMappingContext(nullptr),
    DroneRollAction(nullptr),
    isPlayerPawnState(true),
    DroneUpAction(nullptr),
    DroneDownAction(nullptr)
{

}

void APlayerPawnController::BeginPlay()
{
    Super::BeginPlay();

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem< UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
}


void APlayerPawnController::togglestate()
{
    if (isPlayerPawnState)
    {
        isPlayerPawnState = false;

        if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem< UEnhancedInputLocalPlayerSubsystem>())
            {
                if (InputMappingContext)
                {
                    Subsystem->AddMappingContext(DroneInputMappingContext, 1);
                }
            }
        }

    }
    else
    {
        isPlayerPawnState = true;

        if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem< UEnhancedInputLocalPlayerSubsystem>())
            {
                if (InputMappingContext)
                {
                    Subsystem->RemoveMappingContext(DroneInputMappingContext);
                }
            }
        }
    }
}

bool APlayerPawnController::GetisPlayerPawnState()
{
    return isPlayerPawnState;
}