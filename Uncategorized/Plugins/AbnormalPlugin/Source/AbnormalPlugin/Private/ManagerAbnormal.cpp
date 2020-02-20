// Fill out your copyright notice in the Description page of Project Settings.

#include "ManagerAbnormal.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"

// Sets default values
AManagerAbnormal::AManagerAbnormal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AManagerAbnormal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AManagerAbnormal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AManagerAbnormal::ExecuteTrigger(const TArray<FString>&  AbnormalTaskNames)
{
	for(auto& TaskName : AbnormalTaskNames)
	{
		for(auto& Item : Abnormals)
		{
			if(Item.Name.Equals(TaskName, ESearchCase::IgnoreCase))
			{
				if(Item.AbnormalActor)
				{
					// ExecuteBinding(Item.AbnormalActor, Item.TargetActors);
					ExecuteSpawnAndBinding(Item.AbnormalClass, Item.TargetActors);
					// ExecutePlay(Item.AbnormalActor);
				}
			}
		}
	}
}

void AManagerAbnormal::ExecuteTrigger1(const TArray<FString>&  AbnormalTaskNames)
{
	for(auto& TaskName : AbnormalTaskNames)
	{
		if(auto Item = Abnormalss.Find(TaskName))
		{
			if(Item)
			{
				if(Item->bDynamicGenerateActor)
				{
					auto Class = Item->AbnormalClass.Get();
					if(Class)
					{
						ExecuteSpawnAndBinding(Class, Item->TargetActors);
					}
				}
				else
				{
					ExecuteBinding(Item->AbnormalActor, Item->TargetActors);
					ExecutePlay(Item->AbnormalActor);
				}
			}
		}
	}
}

void AManagerAbnormal::ExecuteTrigger2(const TArray<FString>&  AbnormalTaskNames)
{
	for(auto& TaskName : AbnormalTaskNames)
	{
		if(auto ItemArr = Abnormalsss.Find(TaskName))
		{
			if(ItemArr)
			{
				for(auto& Item : ItemArr->Abnormals)
				{
					if(Item.bDynamicGenerateActor)
					{
						if(auto Class = Item.AbnormalClass.Get())
						{
							ExecuteSpawnAndBinding(Class, Item.TargetActors);
						}
					}
					else
					{
						ExecuteBinding(Item.AbnormalActor, Item.TargetActors);
						ExecutePlay(Item.AbnormalActor);
					}
				}
			}
		}
	}
}

void AManagerAbnormal::ExecuteBinding_Implementation(AAbnormalActor* BindingActor, const TArray<class AActor*>& TargetActors)
{
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	// Rules.LocationRule = EAttachmentRule::KeepRelative;
	// Rules.RotationRule = EAttachmentRule::KeepRelative;
	// Rules.ScaleRule = EAttachmentRule::KeepRelative;
	
	for(auto& TargetActor : TargetActors)
	{
		if(BindingActor)
		{
			BindingActor->AttachToActor(TargetActor, Rules);
		}
	}
}

void AManagerAbnormal::ExecuteSpawnAndBinding_Implementation(UClass* AbnormalClass, const TArray<class AActor*>& TargetActors)
{
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	for(auto& TargetActor : TargetActors)
	{
		auto BindingActor = GetWorld()->SpawnActor<AAbnormalActor>(AbnormalClass, FVector(0, 0, 0), FRotator(0, 0, 0), FActorSpawnParameters());
		if(BindingActor)
		{
			BindingActor->AttachToActor(TargetActor, Rules);
			BindingActor->StartPlaySequence();
		}
	}	
}

void AManagerAbnormal::ExecutePlay(AAbnormalActor* BindingActor)
{
	BindingActor->StartPlaySequence();
}