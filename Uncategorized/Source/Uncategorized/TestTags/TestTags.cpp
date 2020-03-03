// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTags.h"

// Sets default values
ATestTags::ATestTags()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FString TagName = FString(TEXT("Tag1"));
	this->Tags.Emplace(FName(TEXT("TestTags")));
	this->Tags.Emplace(FName(*TagName));
}

// Called when the game starts or when spawned
void ATestTags::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestTags::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

