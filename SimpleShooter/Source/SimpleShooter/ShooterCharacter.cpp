// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);   // ocultamos el arma del esqueleto (mesh) para poner la que spawneamos como hija y así poder cambiarla etc
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));  // no usamos setupattachment porque eso se usa en el constructor. Aquí al estar enlazando el arma en run time debemos usar otra función
	Gun->SetOwner(this);  // aunq no haya jerarquía en las transformadas de los actores, sí que pueden tener dueño entre ellos (importante para multiplayer, hacer/recibir daño y para poder acceder al personaje a través del arma)

	Cast<APlayerController>(GetController());
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health/MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this,  &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	// PlayerInputComponent->BindAxis(TEXT("LookUp"), this, AShooterCharacter::MoveForward);  esta forma es válida pero innecesaria ya que solo estamos llamando a una función ya definida
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);  // cuando hacemos override de un método, si no llamamos al super, el compilador olvida el funcionamiento de la clase padre y ahí pueden haber cosas importantes
	
	// Cogemos damagetoapply y no damageamount debido a que podriamos aplicar distintos tipos de daño que modifiquen ese daño como puede ser TSubclassOf<UDamageType> en el damage event de gun
	DamageToApply = FMath::Min(Health, DamageToApply); // debemos asegurarnos que el daño aplicado no sea mayor que la cantidad de vida que tenemos restante
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

	if(IsDead())
	{
		ASimpleShooterGameModeBase* GameMode =  GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();  // me interesa castear al root game mode para poder usar distintos game modes hijos en distintos niveles como killemall etc. En definitiva, evito limitar mis posibilidades.

		if(GameMode != nullptr)
		{
			// indispensable hacer esto antes de detachear del controller porque si no, el juego no reconoce el controlador y no podremos terminar la partida en el caso de que terminara (muere el jugador, mueren todos los enemigos... etc) 
			GameMode->PawnKilled(this);  // esto llama al pawnkilled de killemall en este caso porque es el game mode bp del nivel ahora mismo, pero si lo cambiaramos llamaría a otro game mode hijo
			// esto ocurre ya que estamos llamando a killemallgamemode como asimpleshootergamemodebase al ser el padre se puede hacer porque en definitiva son lo mismo y por eso toma prioridad el pawnkilled de killemallgamemode y no el del padre
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
	return DamageToApply;

}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);   // no necesitamos añadir el delta porque ya presupone que es un Rate no una cantidad de movimiento concreta a diferencia de addcontrollerpitchinput
}

// Implementamos estos dos metodos para el mando porque, a diferencia del ratón, no es frame rate independent
void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());  // AxisValue * Rotation = Speed (m/s)  --  AxisValue * Rotation * DeltaTime = Distance (m)
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}

// void AShooterCharacter::LookUp(float AxisValue)
// {
// 	AddControllerPitchInput(AxisValue);
// }

