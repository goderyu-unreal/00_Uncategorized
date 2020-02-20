// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalActor.generated.h"

UCLASS()
class ABNORMALPLUGIN_API AAbnormalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbnormalActor();

	// UFUNCTION(BlueprintImplementableEvent, Category = Abnormal)
	// void ExecuteTrigger(const TArray<class AActor*>& TargetActors);

	UFUNCTION(BlueprintImplementableEvent, Category = Abnormal)
	void StartPlaySequence();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
