// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "Engine.h"

// Sets default values
AFindPlayerLocation::AFindPlayerLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFindPlayerLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFindPlayerLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector MyPlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("PlayerLocation is %s"), *MyPlayerLocation.ToString()));
	}

}

