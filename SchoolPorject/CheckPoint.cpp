// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"
#include "SchoolPorjectPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckPointZone = CreateDefaultSubobject<UBoxComponent>("CheckPointZone");
	CheckPointZone->SetBoxExtent(FVector(100, 100, 100));
	RootComponent = CheckPointZone;
	bIsOnce = false;
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ASchoolPorjectPawn* SchoolPorjectPawn = Cast<ASchoolPorjectPawn>(PlayerPawn);


	if (SchoolPorjectPawn->bIsCheckHalfPoint())
	{
		if (!bIsOnce)
		{
			CheckPointZone->SetGenerateOverlapEvents(true);
			bIsOnce = true;
		}
	}

}

void ACheckPoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	CheckPointZone->SetGenerateOverlapEvents(false);
}

