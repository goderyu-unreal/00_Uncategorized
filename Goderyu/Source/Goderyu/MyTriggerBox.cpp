// Fill out your copyright notice in the Description page of Project Settings.
#include "MyTriggerBox.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"

#define PrintFString(text, fstring) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring));

AMyTriggerBox::AMyTriggerBox() {
    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapEnd);
}

void AMyTriggerBox::BeginPlay() {
    Super::BeginPlay();

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);

}

void AMyTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) {
    if (OtherActor && (OtherActor != this) && OtherActor == SpecificActor ) {
        PrintFString("%s has entered the box", *OtherActor->GetName());
    }
}

void AMyTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) {
    if (OtherActor && (OtherActor != this) && OtherActor == SpecificActor) {
        PrintFString("%s has left the box", *OtherActor->GetName());
    }
}

