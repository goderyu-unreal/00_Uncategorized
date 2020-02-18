// Fill out your copyright notice in the Description page of Project Settings.


#include "GetNumPawns.h"
#include "Engine.h"

// Sets default values
AGetNumPawns::AGetNumPawns()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGetNumPawns::BeginPlay()
{
	Super::BeginPlay();
	
	// Note: 4.23开始已经不想支持该方法了
	int32 NumPawns = GetWorld()->GetNumPawns();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Pawns num is %d"), NumPawns));
	}

}

// Called every frame
void AGetNumPawns::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

