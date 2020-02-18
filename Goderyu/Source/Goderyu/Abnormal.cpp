// Fill out your copyright notice in the Description page of Project Settings.

#include "Abnormal.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"

// class UMovieSceneSequencePlayer;
// class ULevelSequencePlayer;

// Sets default values
AAbnormal::AAbnormal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAbnormal::BeginPlay()
{
	/**
	 * Create a new level sequence player.
	 *
	 * @param WorldContextObject Context object from which to retrieve a UWorld.
	 * @param LevelSequence The level sequence to play.
	 * @param Settings The desired playback settings
	 * @param OutActor The level sequence actor created to play this sequence.
	 */
	 // UFUNCTION(BlueprintCallable, Category="Game|Cinematic", meta=(WorldContext="WorldContextObject", DynamicOutputParam="OutActor"))
	 // static ULevelSequencePlayer* CreateLevelSequencePlayer(UObject* WorldContextObject, ULevelSequence* LevelSequence, FMovieSceneSequencePlaybackSettings Settings, ALevelSequenceActor*& OutActor);
	Super::BeginPlay();
	class ALevelSequenceActor* LSActor;
	FMovieSceneSequencePlaybackSettings Settings;
	auto LSPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, LevelSequence, Settings, LSActor);
	LSPlayer->Play();
}

// Called every frame
void AAbnormal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}