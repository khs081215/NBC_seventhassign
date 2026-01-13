// Fill out your copyright notice in the Description page of Project Settings.


#include "NBCGameMode.h"
#include "PlayerPawn.h"
#include "PlayerPawnController.h"

ANBCGameMode::ANBCGameMode()
{
    DefaultPawnClass = APlayerPawn::StaticClass();
    PlayerControllerClass = APlayerPawnController::StaticClass();
}