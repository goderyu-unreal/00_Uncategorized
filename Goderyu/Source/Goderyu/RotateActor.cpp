// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateActor.h"

// Sets default values
ARotateActor::ARotateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotateActor::BeginPlay()
{
	Super::BeginPlay();
	
	Dimensions = FVector(300, 0, 0);
	AxisVector = FVector(0, 0, 1);
	Multiplier = 50.f;
}

// Called every frame
void ARotateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FVector NewLocation;
        UWorld* World = GetWorld();
	if(World != nullptr) {
		NewLocation = 
			World->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	}

	AngleAxis += DeltaTime * Multiplier;

	if(AngleAxis >= 360.0f)
	{
		AngleAxis = 0;
	}

	FVector RotatorValue = Dimensions.RotateAngleAxis(AngleAxis, AxisVector);

	NewLocation.X += RotatorValue.X;
	NewLocation.Y += RotatorValue.Y;
	NewLocation.Z += RotatorValue.Z;
	FRotator NewRotation = FRotator(0, AngleAxis, 0);

	FQuat QuatRotation = FQuat(NewRotation);

	SetActorLocationAndRotation(NewLocation, QuatRotation, false, 0, ETeleportType::None);
}

