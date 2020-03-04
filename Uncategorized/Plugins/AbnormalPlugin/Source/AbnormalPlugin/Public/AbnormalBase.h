// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalBase.generated.h"

UCLASS()
class ABNORMALPLUGIN_API AAbnormalBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbnormalBase();

	/// 播放Actor中ActorSequenceComponent组件包含的动画
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Abnormal)
	void StartPlaySequence();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
