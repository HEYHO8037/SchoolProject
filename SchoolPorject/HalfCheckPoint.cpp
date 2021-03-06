// Fill out your copyright notice in the Description page of Project Settings.


#include "HalfCheckPoint.h"

// Sets default values
AHalfCheckPoint::AHalfCheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HalfCheckPointZone = CreateDefaultSubobject<UBoxComponent>("HalfCheckPointZone");
	HalfCheckPointZone->SetBoxExtent(FVector(100, 100, 100));
}

// Called when the game starts or when spawned
void AHalfCheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHalfCheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHalfCheckPoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	this->Destroy();
}
