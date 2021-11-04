// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishPoint.h"
#include "SchoolPorjectPawn.h"

// Sets default values
AFinishPoint::AFinishPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FinishPointZone = CreateDefaultSubobject<UBoxComponent>("FinishPointZone");
	FinishPointZone->SetBoxExtent(FVector(100, 100, 100));
}

// Called when the game starts or when spawned
void AFinishPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFinishPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishPoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	this->Destroy();
}

