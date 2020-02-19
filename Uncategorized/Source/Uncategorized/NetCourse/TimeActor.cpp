// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeActor.h"
#include "Engine.h"
#include "TimerManager.h"

// Sets default values
ATimeActor::ATimeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimeActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ATimeActor::RepeatingFunction, 2.f, true, 5.f);
	
}

// Called every frame
void ATimeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimeActor::RepeatingFunction()
{
	static int32 Count = 0;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Hello Unreal %d"), ++Count), false);
	}
	
}