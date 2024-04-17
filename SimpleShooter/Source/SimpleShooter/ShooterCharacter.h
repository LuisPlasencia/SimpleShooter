// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)  // more than blueprintcallable (quita la necesidad de execution ping), suele ir con const - si es pure no tiene execution ping al no tener ningun impacto en nada de lo que llama, solo en lo que devuelve
	bool IsDead() const;

	UFUNCTION(BlueprintPure)  
	float GetHealthPercent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// no es necesario poner virtual delante, eso solo es necesario en el padre para que si llamas a una clase con esta función no coja la del padre auto sino la del hijo si la implementa, pero tampoco hace daño

	void Shoot();  // es pública porque el BTTask necesita llamarla para que el personaje controlado por IA pueda disparar


private:
	void MoveForward(float AxisValue);   //ctr+shift+p - create implementation c++ helper extension   alt+o para ir al fichero cpp
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	// void LookUp(float AxisValue);

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass; // editdefaultsonly para no poder cambiarlo en run time

	UPROPERTY()
	AGun* Gun;

	


};
