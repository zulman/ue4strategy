// Fill out your copyright notice in the Description page of Project Settings.

#include "R3.h"
#include "TestPawn.h"


// Sets default values
ATestPawn::ATestPawn()
{
	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 400.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//// Create a dummy root component we can attach things to.
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//// Create a camera and a visible object
	//UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	//OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	//// Attach our camera and visible object to our root component. Offset and rotate the camera.
	//OurCamera->AttachTo(RootComponent);
	//OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	//OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	//OurVisibleComponent->AttachTo(RootComponent);

	UE_LOG(LogTemp, Warning, TEXT("cpp pawn is wokring"));
}

// Called when the game starts or when spawned
void ATestPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//// Handle growing and shrinking based on our "Grow" action
	//{
	//	float CurrentScale = OurVisibleComponent->GetComponentScale().X;
	//	if (bGrowing)
	//	{
	//		// Grow to double size over the course of one second
	//		CurrentScale += DeltaTime;
	//	}
	//	else
	//	{
	//		// Shrink half as fast as we grow
	//		CurrentScale -= (DeltaTime * 0.5f);
	//	}
	//	// Make sure we never drop below our starting size, or increase past double size.
	//	CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
	//	OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));
	//}

	//// Handle movement based on our "MoveX" and "MoveY" axes
	//{
	//	if (!CurrentVelocity.IsZero())
	//	{
	//		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	//		SetActorLocation(NewLocation);
	//	}
	//}

	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;        //Zoom out over a quarter of a second
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		OurCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}

	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	//Rotate our camera's pitch, but limit it so we're always looking downward
	{
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation);
	}

	//Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 100 units per second
			MovementInput = MovementInput.SafeNormal() * 100.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void ATestPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	//// Respond when our "Grow" key is pressed or released.
	//InputComponent->BindAction("Grow", IE_Pressed, this, &ATestPawn::StartGrowing);
	//InputComponent->BindAction("Grow", IE_Released, this, &ATestPawn::StopGrowing);

	//// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	//InputComponent->BindAxis("MoveX", this, &ATestPawn::Move_XAxis);
	//InputComponent->BindAxis("MoveY", this, &ATestPawn::Move_YAxis);

	//Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ATestPawn::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &ATestPawn::ZoomOut);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &ATestPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATestPawn::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &ATestPawn::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &ATestPawn::YawCamera);

}

//void ATestPawn::Move_XAxis(float AxisValue)
//{
//	// Move at 100 units per second forward or backward
//	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
//}
//
//void ATestPawn::Move_YAxis(float AxisValue)
//{
//	// Move at 100 units per second right or left
//	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
//}
//
//void ATestPawn::StartGrowing()
//{
//	bGrowing = true;
//}
//
//void ATestPawn::StopGrowing()
//{
//	bGrowing = false;
//}


//Input functions
void ATestPawn::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ATestPawn::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ATestPawn::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void ATestPawn::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void ATestPawn::ZoomIn()
{
	bZoomingIn = true;
}

void ATestPawn::ZoomOut()
{
	bZoomingIn = false;
}