// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        // GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation()); 
    }
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);



// ASI LO HACIAMOS ANTES DE IMPLEMENTAR BTService en el AI blackboard lo cual es más eficiente al solo comprobar las cosas cuando debe y con menos frecuencia así que mejora el performance
    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    // if(LineOfSightTo(PlayerPawn))
    // {
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    // } 
    // else
    // {
    //     GetBlackboardComponent()->ClearValue("PlayerLocation"); 
    // }


 // ASI LO HACIAMOS ANTES DE IMPLEMENTAR AI BLACKBOARD
    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    // if(LineOfSightTo(PlayerPawn))
    // {
    //     MoveToActor(PlayerPawn, AcceptanceRadius); // que se quede a 2 metros del jugador
    //     SetFocus(PlayerPawn);
    // } 
    // else
    // {
    //     ClearFocus(EAIFocusPriority::Gameplay);
    //     StopMovement();
    // }

    
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if(ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }
    return true;   // si el pawn ha sido detatcheado del controller no reconocerá ningún pawn esta clase así que podemos darlo por muerto a dicho npc
}
