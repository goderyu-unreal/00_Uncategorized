// Fill out your copyright notice in the Description page of Project Settings.


#include "SetCameraViewTarget.h"

// #include "Engine.h"
#include "Components/BillboardComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASetCameraViewTarget::ASetCameraViewTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/**
	 * 组件的创建要放在构造函数中，放在BeginPlay崩溃一直...
	 * 看了官方CameraActor的源码，人家就是放在构造函数中创建组件，设置参数的
	 * 
	 */
	MySceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("MySceneComp"));
	RootComponent = MySceneComp;

	MyCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCameraComp"));
	MyCameraComp->SetupAttachment(MySceneComp);

}

// Called when the game starts or when spawned
void ASetCameraViewTarget::BeginPlay()
{
	Super::BeginPlay();
	// SetActorLocationAndRotation(FVector(-300, -300, 300), FQuat(FRotator(0, 45, 45)));

	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 5.f);
	// if (GEngine) {
		// GEngine->GetFirstPlayerController()->SetViewTargetWithBlend();
		// GEngine->GetFirstLocalPlayerController(GetWorld())->SetViewTargetWithBlend(this, 5.f);
	// }
}

// Called every frame
void ASetCameraViewTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

