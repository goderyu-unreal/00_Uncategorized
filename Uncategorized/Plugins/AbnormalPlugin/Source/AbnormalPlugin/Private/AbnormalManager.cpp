// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalManager.h"
#include "AbnormalPlugin.h"
#include "EngineUtils.h"
#include "Engine/EngineTypes.h"
#include "Engine/TargetPoint.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AAbnormalManager::AAbnormalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AAbnormalManager *AAbnormalManager::GetInstance(const UObject *WorldContextObject)
{
	static AAbnormalManager *Instance = nullptr;
	if (!Instance->IsValidLowLevel())
	{
		if (auto It = TActorIterator<AAbnormalManager>(WorldContextObject->GetWorld()))
		{
			Instance = *It;
		}
		else
		{
			Instance = WorldContextObject->GetWorld()->SpawnActor<AAbnormalManager>();
			UE_LOG(LogAbnormalPlugin, Error, TEXT("Dynamic Spawn AbnormalManager, No data is filled in"));
		}
	}
	return Instance;
}


void AAbnormalManager::TriggerTask_Implementation(const FString& AbnormalId, const FString& AbnormalTaskName, const TArray<FTransform>& TargetTransforms)
{
	if (auto AbnormalTask = Abnormals.Find(AbnormalTaskName))
	{
		for (auto &AbnormalInfo : AbnormalTask->AbnormalsInfo)
		{
			auto SpawnedTargetActors = SpawnTargetActor(AbnormalId, TargetTransforms);
			AbnormalInfo.TargetActors.Append(SpawnedTargetActors);
			if (AbnormalInfo.bDynamicGenerateActor)
			{
				if (auto Class = AbnormalInfo.AbnormalClass.Get())
				{
					SpawnAndBindingAbnormalActorToTargetActors(AbnormalId, Class, AbnormalInfo.TargetActors);
				}
				else
				{
					UE_LOG(LogAbnormalPlugin, Warning, TEXT("填表信息中没有为%s非正常条目指定非正常Actor"), *AbnormalTaskName);
				}
			}
			else
			{
				if (AbnormalInfo.AbnormalActor)
				{
					// TODO 将逻辑该为传递目标点数组，找到第一个有效元素挂载后返回。
					if (AbnormalInfo.TargetActors.Num() > 0 && AbnormalInfo.TargetActors.Last())
					{
						BindingAbnormalActorToTargetActors(AbnormalId, AbnormalInfo.AbnormalActor, AbnormalInfo.TargetActors.Last());
					}
					else
					{
						UE_LOG(LogAbnormalPlugin, Warning, TEXT("目标点集合没有有效元素"));
						
					}
				}
				else
				{
					UE_LOG(LogAbnormalPlugin, Warning, TEXT("填表信息中没有为%s非正常条目指定具体的非正常Actor"), *AbnormalTaskName);
				}
			}
		}
	}
}

void AAbnormalManager::SpawnAndBindingAbnormalActorToTargetActors_Implementation(const FString& AbnormalId, UClass* AbnormalClass, const TArray<class AActor*>& TargetActors)
{
	auto AttachRule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	for (const auto& TargetActor : TargetActors)
	{
		if(auto AbnormalActor = GetWorld()->SpawnActor<AAbnormalBase>(AbnormalClass, FVector::ZeroVector, FRotator::ZeroRotator, FActorSpawnParameters()))
		{
			AbnormalActor->Tags.Emplace(FName(*AbnormalId));
			AbnormalActor->AttachToActor(TargetActor, AttachRule);
			// 换成委托，通知已经绑定结束，在播放动画前留一个余地做额外的准备操作
			OnBindingActorFinished.Broadcast();
			// AbnormalActor->StartPlaySequence();
		}
	}
}

// TODO 貌似可以和动态生成绑定函数整合成一个函数，不过也不好
void AAbnormalManager::BindingAbnormalActorToTargetActors_Implementation(const FString& AbnormalId, AAbnormalBase* AbnormalActor, AActor* TargetActor)
{
	auto AttachRule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	AbnormalActor->AttachToActor(TargetActor, AttachRule);
	OnBindingActorFinished.Broadcast();
}

const TArray<class AActor*> AAbnormalManager::SpawnTargetActor_Implementation(const FString& AbnormalId, const TArray<FTransform>& TargetTransforms)
{
	TArray<class AActor*> TargetActors;
	for (const auto& TargetTransform : TargetTransforms)
	{
		if(auto TargetActor = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), TargetTransform, FActorSpawnParameters()))
		{
			TargetActor->Tags.Emplace(FName(*AbnormalId));
			TargetActors.Emplace(TargetActor);
		}
	}
	return TargetActors;
}


bool AAbnormalManager::DestroyAbnormalActorsById_Implementation(const FString& AbnormalId)
{
	TArray<AActor*> ActorsWithAbnormalIdTag;
	UGameplayStatics::GetAllActorsWithTag(this, FName(*AbnormalId), OUT ActorsWithAbnormalIdTag);
	bool bAllDestroy = true;
	for (auto& Actor : ActorsWithAbnormalIdTag)
	{
		if (Actor)
		{
			bAllDestroy &= Actor->Destroy();
		}
	}
	return bAllDestroy;
}

void AAbnormalManager::PostEditChangeProperty(struct FPropertyChangedEvent &PropertyChangedEvent)
{
	auto PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AAbnormalManager, Abnormals))
	{
		UE_LOG(LogAbnormalPlugin, Error, TEXT("Trigger Property Changed"));
	}
	// if (PropertyName == GET_MEMBER_NAME_CHECKED(AAbnormalManager, bDynamic))
	// {
	// 	if (bDynamic)
	// 	{
	// 		//显示V1，隐藏V2
	// 		for (TFieldIterator<UProperty> ProIt(AAbnormalManager::StaticClass()); ProIt; ++ProIt)
	// 		{
	// 			auto Pro = *ProIt;
	// 			if (Pro->GetNameCPP().Equals("V1"))
	// 			{
	// 				// auto BoolPro = Cast<UBoolProperty>(Pro);
	// 				// if (BoolPro)
	// 				// {
	// 				// 	V1FlagValue = BoolPro->GetPropertyFlags();
	// 				// 	BoolPro->SetPropertyFlags(CPF_Edit);
	// 				// 	BoolPro->SetMetaData("Category", TEXT("Abnormal"));
	// 				// }
	// 				// V1FlagValue = Pro->GetPropertyFlags();
	// 				Pro->SetPropertyFlags(CPF_Edit);
	// 				Pro->SetMetaData("Category", TEXT("Abnormal"));
	// 				continue;
	// 			}
	// 			else if (Pro->GetNameCPP().Equals("V2"))
	// 			{
	// 				// auto BoolPro = Cast<UBoolProperty>(Pro);
	// 				// if (BoolPro)
	// 				// {
	// 				// 	V1FlagValue = BoolPro->GetPropertyFlags(); //6755469234274817
	// 				// 	BoolPro->ClearPropertyFlags(CPF_Edit);
	// 				// 	BoolPro->RemoveMetaData("Category");
	// 				// }
	// 				// V2FlagValue = Pro->GetPropertyFlags();
	// 				Pro->ClearPropertyFlags(CPF_Edit);
	// 				Pro->RemoveMetaData("Category");
	// 				continue;
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		//隐藏V1，显示V2
	// 		for (TFieldIterator<UProperty> ProIt(AAbnormalManager::StaticClass()); ProIt; ++ProIt)
	// 		{
	// 			auto Pro = *ProIt;
	// 			if (Pro->GetNameCPP().Equals("V1"))
	// 			{
	// 				// auto BoolPro = Cast<UBoolProperty>(Pro);
	// 				// if (BoolPro)
	// 				// {
	// 				// 	V1FlagValue = BoolPro->GetPropertyFlags(); //6755469234274817
	// 				// 	BoolPro->ClearPropertyFlags(CPF_Edit);
	// 				// }
	// 				// V1FlagValue = Pro->GetPropertyFlags(); //6755469234274817
	// 				Pro->ClearPropertyFlags(CPF_Edit);
	// 				Pro->RemoveMetaData("Category");
	// 				continue;
	// 			}
	// 			else if (Pro->GetNameCPP().Equals("V2"))
	// 			{
	// 				// auto BoolPro = Cast<UBoolProperty>(Pro);
	// 				// if (BoolPro)
	// 				// {
	// 				// 	V2FlagValue = BoolPro->GetPropertyFlags(); //6755469234274816
	// 				// 	BoolPro->SetPropertyFlags(CPF_Edit);
	// 				// 	BoolPro->SetMetaData("Category", TEXT("Abnormal"));
	// 				// }
	// 				// V2FlagValue = Pro->GetPropertyFlags(); //6755469234274816
	// 				Pro->SetPropertyFlags(CPF_Edit);
	// 				Pro->SetMetaData("Category", TEXT("Abnormal"));
	// 				continue;
	// 			}
	// 		}
	// 	}
	// }
}

// Called when the game starts or when spawned
void AAbnormalManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbnormalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

