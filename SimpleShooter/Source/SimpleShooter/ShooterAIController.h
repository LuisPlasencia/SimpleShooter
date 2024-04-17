// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsDead() const;

protected:
	virtual void BeginPlay() override;   // el override no es necesario, pero si nos equivocamos con el nombre nos avisará de que no estamos overrideando nada porque en el padre no hay ninguna función con dicho nombre así que es buena práctica

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

}; 
