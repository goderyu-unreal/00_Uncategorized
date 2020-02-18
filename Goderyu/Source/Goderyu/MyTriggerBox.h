// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class GODERYU_API AMyTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
	protected:
	virtual void BeginPlay() override;

public:
	AMyTriggerBox();
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	class AActor* SpecificActor;
};
