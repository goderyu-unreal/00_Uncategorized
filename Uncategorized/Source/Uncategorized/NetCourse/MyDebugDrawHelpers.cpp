// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDebugDrawHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/Canvas.h"

// Sets default values
AMyDebugDrawHelpers::AMyDebugDrawHelpers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyDebugDrawHelpers::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	void DrawDebugBox(const UWorld *InWorld, 
	const FVector &Center,
	const FVector &Extent,
	const FColor &Color,
	bool bPersistentLines = false,
	float LifeTime = (-1.0F),
	uint8 DepthPriority = (uint8)'\000',
	float Thickness = (0.0F))
	
	bPersistentLines 需要设置为true才能看见绘制的Box
	persistent 持续的
	*/
	DrawDebugBox(GetWorld(), FVector(-400, 0, 200), FVector(100, 100, 70), FColor::Turquoise, true, -1.f, 0, 3.f);

	DrawDebugCircle(GetWorld(), FVector(-400, 0, 500), 50.f, 300, FColor::Yellow, true, -1, 0, 5.f, FVector(1, 0, 0), FVector(0, 0, 1), false);

	DrawDebugCylinder(GetWorld(), FVector(-400, 100, 400), FVector(-400, 100, 600), 70, 5, FColor::Silver, true, -1, 0, 5.f);

	// UCanvas* Canvas = CreateDefaultSubobject<UCanvas>(TEXT("My Canvas"));
	// Canvas = NewObject<UCanvas>();
	// DrawDebugCanvasLine(Canvas, FVector(-400, 300, 200), FVector(-400, 300, 400), FLinearColor(FColor::Orange));

	DrawDebugCapsule(GetWorld(), FVector(-400, 400, 200), 40, 25, FQuat(FRotator(90, 0, 0)), FColor::Transparent, true, -1, 0, 3.f);

	DrawDebugCrosshairs(GetWorld(), FVector(-400, 0, 500), FRotator(45, 45, 45), 200, FColor::Emerald, true, -1, 0);

	DrawDebugString(GetWorld(), FVector(-400, 0, 200), FString("My DrawDebugHelpers"), 0, FColor::Red, -1, true, 2);

	/*
		cm为单位
	*/
	DrawDebugSolidPlane(GetWorld(), FPlane(FVector(100, 100, 100), FVector(1, 1, 1)), FVector(-400, 0, 200), FVector2D(100, 50), FColor::Cyan, true, -1, 0);
}

// Called every frame
void AMyDebugDrawHelpers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

