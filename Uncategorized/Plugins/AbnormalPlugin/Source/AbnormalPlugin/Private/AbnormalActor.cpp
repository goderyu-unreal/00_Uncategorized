// Fill out your copyright notice in the Description page of Project Settings.

#include "AbnormalActor.h"


// Sets default values
AAbnormalActor::AAbnormalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAbnormalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbnormalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// void AAbnormalActor::ExecuteTrigger(const TArray<AActor*>&  TargetActors)
// {
// 	for(auto& TargetActor : TargetActors)
// }
