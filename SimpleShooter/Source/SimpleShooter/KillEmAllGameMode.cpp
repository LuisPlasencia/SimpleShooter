// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"


void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    // UE_LOG(LogTemp, Warning, TEXT("A pawn was killed!"));

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());  // comprobamos si el pawn matado es el jugador, casteamos a padre por si tenemos distintos controladores en distintos niveles, el método al que llame será el override por defecto
    // el controller debe manejar el end game y las cuestiones de UI ya que si esto fuera un multijugador no existiría game mode como tal. Cada jugador implementaría su UI a través de su controlador.
    // por esto unreal hasta nos pone métodos específicos de manejo de nivel (endgame) en el controlador como GameHasEnded, el cual está en PlayerController, la clase padre, y podemos overridearlo
    if(PlayerController != nullptr)
    {
        // PlayerController->GameHasEnded(nullptr, false); en vez de solo notificar al jugador, es mucho mejor notificar a todos los controladores de todos los pawn (tanto IA como jugadores)
        EndGame(false);
    }

    for(AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))  // nos devuelve todos los controller de AI
    {
        if(!Controller->IsDead())
        {
            return;  // queda algún npc vivo así que no podemos dar por terminado el game
        }
    }
    EndGame(true);

}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    for(AController* Controller : TActorRange<AController>(GetWorld()))  // nos devuelve todos los controller, ya sea de AI como de jugadores
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;   // con esta línea ya evaluo los 4 casos de abajo 
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);  // el primer parámetro dice donde debería apuntar el focus de la vista o la cámara en el end game -- esto es por si por ejemplo quieres que la cámara siga al campeón en un battle royalle por ejemplo o al tipo que te mató
        
        // if(Controller->IsPlayerController() && bIsPlayerWinner)
        // {
        //     Controller->GameHasEnded(nullptr, true);
        // }

        // if(!Controller->IsPlayerController() && !bIsPlayerWinner)
        // {
        //     Controller->GameHasEnded(nullptr, true);
        // }

        // if(Controller->IsPlayerController() && !bIsPlayerWinner)
        // {
        //     Controller->GameHasEnded(nullptr, false);
        // }

        // if(!Controller->IsPlayerController() && bIsPlayerWinner)
        // {
        //     Controller->GameHasEnded(nullptr, false);
        // }

    }
}
