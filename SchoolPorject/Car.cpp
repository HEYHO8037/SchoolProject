// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	MovementComponent = CreateDefaultSubobject<UCarMovementComponent>("MovementComponent");
	MovementReplicationComponent = CreateDefaultSubobject<UCarMovementReplicationComponent>("MovementReplicationComponent");

}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();
	

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}

}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACar::MoveForward(float Value)
{
	if (!MovementComponent)
		return;

	MovementComponent->SetThrottle(Value);
}

void ACar::MoveRight(float Value)
{
	if (!MovementComponent)
		return;

	MovementComponent->SetSteeringThrow(Value);
}

FString ACar::GetEnumText(ENetRole OwnerRole)
{
	switch (OwnerRole)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	case ROLE_MAX:
		return "Max";
	default:
		return "Error";
	}
}

// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACar::MoveRight);
}

