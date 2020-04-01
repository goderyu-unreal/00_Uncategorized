// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalTaskInfo.h"

// Sets default values
AAbnormalTaskInfo::AAbnormalTaskInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAbnormalTaskInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbnormalTaskInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


#if WITH_EDITOR
void AAbnormalTaskInfo::PostEditChangeProperty(struct FPropertyChangedEvent &PropertyChangedEvent)
{
	auto PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AAbnormalTaskInfo, ExtraAbnormals))
	{
		Abnormals.KeySort([](const FString &A, const FString &B) {
			return A < B;
		});
	}
}
#endif // WITH_EDITOR