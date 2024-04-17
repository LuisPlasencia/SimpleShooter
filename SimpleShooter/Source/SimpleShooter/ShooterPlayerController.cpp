// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"


void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if(HUD != nullptr)
    {
        HUD->AddToViewport();
    }

}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    // UE_LOG(LogTemp, Warning, TEXT("We have finished."));

    HUD->RemoveFromViewport();
    if (bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);  // only certain things can own a widget, a player controller is one of them (this)
        if(WinScreen != nullptr)
        {
            // hemos tenido que añadir en simpleshooter.build.cs un módulo de dependencia extra para que unreal pueda hacer uso de los include que estamos usando
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);  // only certain things can own a widget, a player controller is one of them (this)
        if(LoseScreen != nullptr)
        {
            // hemos tenido que añadir en simpleshooter.build.cs un módulo de dependencia extra para que unreal pueda hacer uso de los include que estamos usando
            LoseScreen->AddToViewport();
        }
    }


    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay); // este método de restartlevel nos lo facilita unreal


}


