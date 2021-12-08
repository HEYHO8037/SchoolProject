// Copyright Epic Games, Inc. All Rights Reserved.

#include "SchoolPorjectPawn.h"
#include "SchoolPorjectWheelFront.h"
#include "SchoolPorjectWheelRear.h"
#include "SchoolPorjectHud.h"
#include "CheckPoint.h"
#include "SchoolPorjectGameMode.h"
#include "HalfCheckPoint.h"
#include "FinishPoint.h"


#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Materials/Material.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

#ifndef HMD_MODULE_INCLUDED
#define HMD_MODULE_INCLUDED 0
#endif

// Needed for VR Headset
#if HMD_MODULE_INCLUDED
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#endif // HMD_MODULE_INCLUDED

const FName ASchoolPorjectPawn::LookUpBinding("LookUp");
const FName ASchoolPorjectPawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

ASchoolPorjectPawn::ASchoolPorjectPawn(const FObjectInitializer& ObjectInitalizer)
	: Super(ObjectInitalizer)
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	
	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = USchoolPorjectWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = USchoolPorjectWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = USchoolPorjectWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = USchoolPorjectWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(0.0f, -40.0f, 120.0f);

	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());

	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	//Setup TextRenderMaterial
	static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("Material'/Engine/EngineMaterials/AntiAliasedTextMaterialTranslucent.AntiAliasedTextMaterialTranslucent'"));
	
	UMaterialInterface* Material = TextMaterial.Object;

	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetTextMaterial(Material);
	InCarSpeed->SetRelativeLocation(FVector(70.0f, -75.0f, 99.0f));
	InCarSpeed->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	InCarSpeed->SetupAttachment(GetMesh());
	InCarSpeed->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetTextMaterial(Material);
	InCarGear->SetRelativeLocation(FVector(66.0f, -9.0f, 95.0f));	
	InCarGear->SetRelativeRotation(FRotator(25.0f, 180.0f,0.0f));
	InCarGear->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	InCarGear->SetupAttachment(GetMesh());

	
	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;

	playerLap = 0;
	bIsHalfPoint = false;
	bIsFinished = false;
	bIsOnce = false;

	CountDownTime = 3;
	InGameTimer = 0.0f;
	

	ConstructorHelpers::FClassFinder<UCountDownWidget> BPWidget(TEXT("/Game/UI/CountDownWidget_BP"));
	CountDownWidget = BPWidget.Class;

	ConstructorHelpers::FClassFinder<UEndRaceWidget> CountBPWidget(TEXT("/Game/UI/EndRaceWidget_BP"));
	EndRaceWidget = CountBPWidget.Class;

	ConstructorHelpers::FClassFinder<UInGameTimerWidget> RaceWidget(TEXT("/Game/UI/InGameTimer"));
	TimerWidget = RaceWidget.Class;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);
}

void ASchoolPorjectPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASchoolPorjectPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASchoolPorjectPawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ASchoolPorjectPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ASchoolPorjectPawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ASchoolPorjectPawn::OnToggleCamera);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASchoolPorjectPawn::OnResetVR); 

}

void ASchoolPorjectPawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ASchoolPorjectPawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ASchoolPorjectPawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ASchoolPorjectPawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ASchoolPorjectPawn::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void ASchoolPorjectPawn::EnableIncarView(const bool bState, const bool bForce)
{
	if ((bState != bInCarCameraActive) || ( bForce == true ))
	{
		bInCarCameraActive = bState;
		
		if (bState == true)
		{
			OnResetVR();
			Camera->Deactivate();
			InternalCamera->Activate();
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}
		
		InCarSpeed->SetVisibility(bInCarCameraActive);
		InCarGear->SetVisibility(bInCarCameraActive);
	}
}


void ASchoolPorjectPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	
	// Update the strings used in the hud (incar and onscreen)
	UpdateHUDStrings();

	// Set the string in the incar hud
	SetupInCarHUD();

	bool bHMDActive = false;
#if HMD_MODULE_INCLUDED
	if ((GEngine->XRSystem.IsValid() == true) && ((GEngine->XRSystem->IsHeadTrackingAllowed() == true) || (GEngine->IsStereoscopic3D() == true)))
	{
		bHMDActive = true;
	}
#endif // HMD_MODULE_INCLUDED
	if (bHMDActive == false)
	{
		if ( (InputComponent) && (bInCarCameraActive == true ))
		{
			FRotator HeadRotation = InternalCamera->GetRelativeRotation();
			HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
			HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
			InternalCamera->SetRelativeRotation(HeadRotation);
		}
	}

	if (!bIsOnce)
	{
		if (this->GetCountDownTime() == 0)
		{
			Enable();
			bIsOnce = true;
		}
		else
		{
			return;
		}
	}
}

void ASchoolPorjectPawn::BeginPlay()
{
	Super::BeginPlay();

	bool bEnableInCar = false;
#if HMD_MODULE_INCLUDED
	bEnableInCar = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
#endif // HMD_MODULE_INCLUDED
	EnableIncarView(bEnableInCar,true);

	Disable();

}

void ASchoolPorjectPawn::OnResetVR()
{
#if HMD_MODULE_INCLUDED
	if (GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->ResetOrientationAndPosition();
		InternalCamera->SetRelativeLocation(InternalCameraOrigin);
		GetController()->SetControlRotation(FRotator());
	}
#endif // HMD_MODULE_INCLUDED
}

void ASchoolPorjectPawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));

	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}	
}

void ASchoolPorjectPawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr) )
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);
		
		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}

void ASchoolPorjectPawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	playerLap++;

	UE_LOG(LogTemp, Warning, TEXT("Collision"));

	if (OtherActor->IsA(AHalfCheckPoint::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Half"));
		bIsHalfPoint = true;
	}

	if (playerLap >= 5 && bIsHalfPoint == true && OtherActor->IsA(AFinishPoint::StaticClass()))
	{
		bIsFinished = true;
		DisableInput(Cast<APlayerController>(this));
		MoveForward(0);

		EndWidget = CreateWidget<UEndRaceWidget>(GetWorld(), EndRaceWidget);
		EndWidget->AddToViewport();
	}
}

bool ASchoolPorjectPawn::bIsCheckHalfPoint()
{
	return bIsHalfPoint;
}

bool ASchoolPorjectPawn::bIsCheckFinished()
{
	return bIsFinished;
}

void ASchoolPorjectPawn::OnUserCountChanged_Implementation(int32 UserCount)
{
	if (UserCount >= 2)
	{
		OnReady();
	}
}

void ASchoolPorjectPawn::OnReady_Implementation()
{
	ASchoolPorjectGameMode* GameMode = Cast<ASchoolPorjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->OnUserReady();
}

void ASchoolPorjectPawn::OnStart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Yeah"));
	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &ASchoolPorjectPawn::CountDownAdvanceTimer, 1.0f, true);
	ShowCountDown();
}

void ASchoolPorjectPawn::Disable_Implementation()
{
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));
}

void ASchoolPorjectPawn::Enable_Implementation()
{
	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
}

void ASchoolPorjectPawn::ShowCountDown()
{
	Widget = CreateWidget<UCountDownWidget>(GetWorld(), CountDownWidget);
	Widget->AddToViewport();
}

void ASchoolPorjectPawn::CountDownAdvanceTimer()
{
	--CountDownTime;
	UE_LOG(LogTemp, Warning, TEXT("0"));

	if (CountDownTime < 1)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimerHandle);
		GetWorldTimerManager().SetTimer(InGameHandle, this, &ASchoolPorjectPawn::StartInGameAdvanceTimer, 0.1f, true);

		RaceTimeWidget = CreateWidget<UInGameTimerWidget>(GetWorld(), TimerWidget);
		RaceTimeWidget->AddToViewport();
	}
}

void ASchoolPorjectPawn::StartInGameAdvanceTimer()
{
	InGameTimer += 0.1f;
}

int32 ASchoolPorjectPawn::GetCountDownTime()
{
	return CountDownTime;
}

float ASchoolPorjectPawn::GetInGameTime()
{
	return InGameTimer;
}

#undef LOCTEXT_NAMESPACE

PRAGMA_ENABLE_DEPRECATION_WARNINGS
