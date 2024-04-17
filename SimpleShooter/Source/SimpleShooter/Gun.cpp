// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Ctr + p  - skeletalmeshcomponent y mirar arriba donde esta la clase : classes/components/skeletalmeshcomponent.h


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));   //  attacheamos el particle system al esqueleto del arma, concretamente al elemento muzzleflashsocket
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
	//	DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());   // la rotacion es contraria porque el efecto del impacto debe ser contra la pared y no hacia ella ya que el disparo rebota en las superficies
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent = FPointDamageEvent(Damage, Hit, ShotDirection, nullptr);   // hay point damage y radial damage (granadas, etc)
			AController *OwnerController = GetOwnerController();   // como a esta función la llamamos previamente en GunTrace(), no volvemos a calcular si devuelve null o no
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}

	// DrawDebugCamera(GetWorld(), CameraLocation, CameraRotation, 90, 2, FColor::Red, true);


	
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController *OwnerController = GetOwnerController();
	if(OwnerController == nullptr) return false;

	FVector CameraLocation;
	FRotator CameraRotation;  // no usamos puntero porque es un TypeDef no una clase ojito
	OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);    // esta función devuelve el punto de vista de la cámara (componente enlazado al personaje) (ver comentarios de la función para más info con f12)
	// para buscar una función  ctr+p y escribimos @ delante

	ShotDirection = -CameraRotation.Vector();
	FVector End = CameraLocation + CameraRotation.Vector() * MaxRange;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());  // esto es necesario porque si no la IA que controla al personaje se dispararía a sí misma ya que el proyectil sale de la cabeza en su caso
	return GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);  //project folder / config/ defaultengine.ini  para ver el nombre del canal del trace que hemos creado
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());   // podriamos castear al shootercharacter pero pawn está más arriba y es la unidad con la que le permite al elemento ser controlado por algo. De ello extraeremos el controller 
	if(OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();

}



