// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateStaticMesh.h"

// Sets default values
ACreateStaticMesh::ACreateStaticMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACreateStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	
	MyStaticMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("My Static Mesh"));

	UE_LOG(LogTemp, Warning, TEXT("UELOG TEST"));

}

// Called every frame
void ACreateStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

