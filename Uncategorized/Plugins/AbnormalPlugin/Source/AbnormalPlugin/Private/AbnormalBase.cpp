// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalBase.h"

// Sets default values
AAbnormalBase::AAbnormalBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAbnormalBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbnormalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
