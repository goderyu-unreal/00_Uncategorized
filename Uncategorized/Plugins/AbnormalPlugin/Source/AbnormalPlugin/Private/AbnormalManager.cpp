// Fill out your copyright notice in the Description page of Project Settings.

#include "AbnormalManager.h"
#include "AbnormalPlugin.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

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

void AAbnormalManager::IntegrateAllAbnormalTaskInfoToAbnormals_Implementation(TMap<FString, FAbnormalsInfo> &AbnormalsMap)
{
	// 整合填表的非正常任务信息
	TArray<AActor *> FindedActors;
	// 这样获取到的Actor，只有和第一个参数，this处于同一个关卡流的才行。
	// 使用加载关卡实例或者打开关卡的方法加载的关卡里面的Actor获取不到的。
	// FIXME 如何完整地获取非正常任务填表信息？
	UGameplayStatics::GetAllActorsOfClass(this, AAbnormalTaskInfo::StaticClass(), OUT FindedActors);
	for (const auto &Actor : FindedActors)
	{
		if (auto AbnormalTaskInfo = Cast<AAbnormalTaskInfo>(Actor))
		{
			// 获取填表的非正常任务信息，并在这里检测是否在整合到Manager的Abnormals时发现已存在，打印出两者所在关卡，供用户修改
			for (const auto &AbnormalTask : AbnormalTaskInfo->ExtraAbnormals)
			{
				// 如果Manager的表中已经存在该条填表中的非正常任务
				if (AbnormalsMap.Contains(AbnormalTask.Key))
				{
					// 获取关卡名，和内容浏览器保持一致的名称，不能直接使用GetLevel()->GetName()，而是使用GetLevel()->GetOuter()->GetName()
					UE_LOG(LogAbnormalPlugin, Warning, TEXT("发现重复填表的非正常任务%s，其所在关卡为%s"), *AbnormalTask.Key, *(AbnormalTaskInfo->GetLevel()->GetOuter()->GetName()));
					continue;
				}
				else
				{
					AbnormalsMap.Emplace(AbnormalTask.Key, AbnormalTask.Value);
					UE_LOG(LogAbnormalPlugin, Log, TEXT("已整合%s非正常任务信息，其填表Actor所在关卡为%s"), *AbnormalTask.Key, *(AbnormalTaskInfo->GetLevel()->GetOuter()->GetName()));
				}
			}
		}
		else
		{
			UE_LOG(LogAbnormalPlugin, Warning, TEXT("获取所有AbnormalTaskInfo的Actor时从AActor转型到AAbnormalTaskInfo失败一个，该Actor所在关卡为%s"), *(Actor->GetLevel()->GetOuter()->GetName()));
		}
	}
}

void AAbnormalManager::TriggerTask_Implementation(const FString& AbnormalId, const FString& AbnormalTaskName, const FTransform& TargetTransform)
{
	// TODO 打印日志增强一下信息，加入非正常任务名，第几个元素有问题，对照前面整合时输出的该条非正常所在关卡，及时修改数据
	if (auto AbnormalTask = Abnormals.Find(AbnormalTaskName))
	{
		for (auto &AbnormalInfo : AbnormalTask->AbnormalsInfo)
		{
			// 执行流程：
			// 1.确定非正常Actor
			if (RegisterAbnormalActor(AbnormalInfo, AbnormalId, AbnormalTaskName) == false) continue;
			// 2.确定挂载点Actor
			if (RegisterTargetActor(AbnormalInfo, AbnormalId, TargetTransform, AbnormalTaskName) == false) continue;
			// 3.执行挂载操作
			AttachAbnormalToTarget(AbnormalInfo);
			// 4.设置相对偏移
			SetAdditiveTransform(AbnormalInfo);
			// 5-8
			InitAbnormalState(AbnormalInfo);
		}
	}
	else
	{
		UE_LOG(LogAbnormalPlugin, Warning, TEXT("外部发送的非正常任务在填表中没有匹配的，外部发送的任务名为%s"), *AbnormalTaskName);
	}
}

bool AAbnormalManager::RegisterAbnormalActor_Implementation(FAbnormalInfo& AbnormalInfo, const FString& AbnormalId, const FString& AbnormalTaskName)
{
	if (AbnormalInfo.AbnormalActorType == EAbnormalActorType::AAT_DynamicGenerate)
	{
		if (AbnormalInfo.AbnormalClass == nullptr)
		{
			UE_LOG(LogAbnormalPlugin, Error, TEXT("填表中未指定有效的非正常蓝图类资源，因此后续操作均不执行，请及时检查，填表的非正常任务名为%s"), *AbnormalTaskName);
			return false;
		}

		if (auto AbnormalActor = SpawnAbnormalActor(AbnormalId, AbnormalInfo.AbnormalClass, FActorSpawnParameters()))
		{
			AbnormalInfo.AbnormalActor = AbnormalActor;
		}
		else
		{
			UE_LOG(LogAbnormalPlugin, Warning, TEXT("动态生成非正常Actor失败，指针为空，后续操作均不执行，请重试，填表的非正常任务名为%s"), *AbnormalTaskName);
			return false;
		}
	}
	else if (AbnormalInfo.AbnormalActorType == EAbnormalActorType::AAT_ExistedInstance)
	{
		// TODO 保留初始状态，以后再次使用该资源时能重置状态，保持效果一致
		if (AbnormalInfo.AbnormalActor == nullptr)
		{
			UE_LOG(LogAbnormalPlugin, Error, TEXT("填表中未指定关卡中放置的有效非正常Actor，因此后续操作均不执行，请及时检查，填表的非正常任务名为%s"), *AbnormalTaskName);
			return false;
		}
	}
	else
	{
		UE_LOG(LogAbnormalPlugin, Warning, TEXT("填表中选择的非正常类型为无，请及时检查，防止外部下发非正常任务时没任何效果，填表的非正常任务名为%s"), *AbnormalTaskName);
		return false;
	}
	return true;
}

bool AAbnormalManager::RegisterTargetActor_Implementation(FAbnormalInfo& AbnormalInfo, const FString& AbnormalId, const FTransform& TargetTransform, const FString& AbnormalTaskName)
{
	if (AbnormalInfo.TargetTransformSource == ETargetTransformSource::TTS_FromOutside)
	{
		if (auto TargetActor = SpawnTargetActor(AbnormalId, TargetTransform, FActorSpawnParameters()))
		{
			AbnormalInfo.TargetActor = TargetActor;
		}
	}
	else if (AbnormalInfo.TargetTransformSource == ETargetTransformSource::TTS_FromTargetActor)
	{
		if (AbnormalInfo.TargetActor == nullptr)
		{
			UE_LOG(LogAbnormalPlugin, Warning, TEXT("填表中未指定有效的挂载点Actor，因此后续操作均不执行，请及时检查，填表的非正常任务名为%s"), *AbnormalTaskName);
			return false;
		}
	}
	return true;
}

void AAbnormalManager::AttachAbnormalToTarget_Implementation(FAbnormalInfo& AbnormalInfo)
{
	if (AbnormalInfo.TargetTransformSource == ETargetTransformSource::TTS_FromSelf)
	{
		AbnormalInfo.AbnormalActor->CustomAttachToTargetActor();
	}
	else
	{
		auto AttachRule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		AbnormalInfo.AbnormalActor->AttachToActor(AbnormalInfo.TargetActor, AttachRule);
	}
}

void AAbnormalManager::SetAdditiveTransform_Implementation(FAbnormalInfo& AbnormalInfo)
{
	if (AbnormalInfo.bWorldTransform)
	{
		AbnormalInfo.AbnormalActor->AddActorWorldTransform(AbnormalInfo.AdditiveTransform);
	}
	else
	{
		// 已测试，在非正常Actor是以SnapToTarget挂载到TargetActor上时
		// 使用AddActorLocalTransform和使用SetActorRelativeTransform效果一样
		AbnormalInfo.AbnormalActor->AddActorLocalTransform(AbnormalInfo.AdditiveTransform);
	}
}

void AAbnormalManager::InitAbnormalState_Implementation(FAbnormalInfo& AbnormalInfo)
{
	// 5.非正常Actor设置播放速率、延迟播放时间
	AbnormalInfo.AbnormalActor->PlayRate = AbnormalInfo.PlayRate;
	AbnormalInfo.AbnormalActor->DelaySeconds = AbnormalInfo.DelaySeconds;
	// 6.非正常Actor进行内部的预设置
	AbnormalInfo.AbnormalActor->PreSet();
	// 7.非正常Actor注册交互式菜单
	AbnormalInfo.AbnormalActor->RegisterMenu();
	// 8.非正常Actor注册预览窗口
	AbnormalInfo.AbnormalActor->RigisterPreviewWindow();
	// 9.非正常Actor执行动画
	AbnormalInfo.AbnormalActor->StartPlaySequence();
}

AActor* AAbnormalManager::SpawnTargetActor(const FString &AbnormalId, const FTransform &TargetTransform, const FActorSpawnParameters& SpawnParameters) const
{
	if (auto TargetActor = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), TargetTransform, SpawnParameters))
	{
		TargetActor->Tags.Emplace(FName(*AbnormalId));
		return TargetActor;
	}
	return nullptr;
}

AAbnormalBase* AAbnormalManager::SpawnAbnormalActor(const FString &AbnormalId, UClass *AbnormalClass, const FActorSpawnParameters& SpawnParameters) const
{
	if (auto AbnormalActor = GetWorld()->SpawnActor<AAbnormalBase>(AbnormalClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters))
	{
		AbnormalActor->Tags.Emplace(FName(*AbnormalId));
		return AbnormalActor;
	}
	return nullptr;
}


bool AAbnormalManager::DestroyAbnormalActorsById_Implementation(const FString &AbnormalId)
{
	TArray<AActor *> ActorsWithAbnormalIdTag;
	UGameplayStatics::GetAllActorsWithTag(this, FName(*AbnormalId), OUT ActorsWithAbnormalIdTag);
	bool bAllDestroy = true;
	for (auto &Actor : ActorsWithAbnormalIdTag)
	{
		if (Actor)
		{
			bAllDestroy &= Actor->Destroy();
		}
	}
	return bAllDestroy;
}

void AAbnormalManager::UpdateAbnormalInfoFromString_Implementation(const FString &AbnormalInfo)
{
	TArray<FString> AbnormalInfos;
	AbnormalInfo.ParseIntoArray(AbnormalInfos, L";");

	for (const auto &EachAbnormal : AbnormalInfos)
	{
		TArray<FString> MessageArray;
		EachAbnormal.ParseIntoArray(MessageArray, L",");
		if (MessageArray.Num() < 2)
			return;
		FTransform TargetTransform = FTransform::Identity;
		// if (MessageArray[0] == "Add")
		// {
		// 	if (MessageArray.Num() != 8 && MessageArray.Num() != 11)
		// 		return;
		// 	//如果生成对象成功，则处理

		// 	FVector TargetLocation = FVector::ZeroVector;
		// 	TargetLocation.X = FCString::Atof(*MessageArray[2]);
		// 	TargetLocation.Y = FCString::Atof(*MessageArray[3]);
		// 	TargetLocation.Z = FCString::Atof(*MessageArray[4]);
		// 	FIntVector OriginLocation = GetWorld()->OriginLocation;
		// 	TargetLocation.X -= OriginLocation.X;
		// 	TargetLocation.Y -= OriginLocation.Y;
		// 	TargetLocation.Z -= OriginLocation.Z;
		// 	TargetTransform.SetLocation(TargetLocation);
		// 	FRotator TargetRotator = FRotator::ZeroRotator;
		// 	TargetRotator.Roll = FCString::Atof(*MessageArray[5]);
		// 	TargetRotator.Pitch = FCString::Atof(*MessageArray[6]);
		// 	TargetRotator.Yaw = FCString::Atof(*MessageArray[7]);
		// 	TargetTransform.SetRotation(TargetRotator.Quaternion());
		// 	if (MessageArray.Num() == 11)
		// 	{
		// 		TargetLocation.X = FCString::Atof(*MessageArray[8]);
		// 		TargetLocation.Y = FCString::Atof(*MessageArray[9]);
		// 		TargetLocation.Z = FCString::Atof(*MessageArray[10]);
		// 		TargetTransform.SetScale3D(TargetLocation);
		// 	}
		// 	TriggerTask("1", MessageArray[1], TargetTransform);
		// }
		if (MessageArray[0] == "AddByID")
		{
			if (MessageArray.Num() < 4)
				return;
			int CoordinateSystem = FCString::Atoi(*MessageArray[1]);
			FString AbnormalTaskId = MessageArray[2];
			FString AbnormalTaskName = MessageArray[3];
			if (MessageArray.Num() >= 5)
			{
				//如果生成对象成功，则处理
				FVector TargetLocation = FVector::ZeroVector;
				if (MessageArray.Num() >= 5)
				{
					TargetLocation.X = FCString::Atof(*MessageArray[4]);
				}
				if (MessageArray.Num() >= 6)
				{
					TargetLocation.Y = FCString::Atof(*MessageArray[5]);
				}
				if (MessageArray.Num() >= 7)
				{
					TargetLocation.Z = FCString::Atof(*MessageArray[6]);
				}
				if (CoordinateSystem != 0)
				{
					TargetLocation.Y *= -1;
					TargetLocation *= 100;
				}
				FIntVector OriginLocation = GetWorld()->OriginLocation;
				TargetLocation.X -= OriginLocation.X;
				TargetLocation.Y -= OriginLocation.Y;
				TargetLocation.Z -= OriginLocation.Z;
				TargetTransform.SetLocation(TargetLocation);
				FRotator TargetRotator = FRotator::ZeroRotator;
				if (MessageArray.Num() >= 8)
				{
					TargetRotator.Roll = FCString::Atof(*MessageArray[7]);
				}
				if (MessageArray.Num() >= 9)
				{
					TargetRotator.Pitch = FCString::Atof(*MessageArray[8]);
				}
				if (MessageArray.Num() >= 10)
				{
					TargetRotator.Yaw = FCString::Atof(*MessageArray[9]);
				}
				TargetTransform.SetRotation(FQuat(TargetRotator));
				FVector TargetScale = FVector(1.0, 1.0, 1.0);
				if (MessageArray.Num() >= 11)
				{
					TargetScale.X = FCString::Atof(*MessageArray[10]);
				}
				if (MessageArray.Num() >= 12)
				{
					TargetScale.Y = FCString::Atof(*MessageArray[11]);
				}
				if (MessageArray.Num() >= 13)
				{
					TargetScale.Z = FCString::Atof(*MessageArray[12]);
				}
				TargetTransform.SetScale3D(TargetScale);
			}
			TriggerTask(AbnormalTaskId, AbnormalTaskName, TargetTransform);
		}
		if (MessageArray[0] == "DeleteByID")
		{
			FString AbnormalTaskId = MessageArray[1];
			DestroyAbnormalActorsById(AbnormalTaskId);
		}
	}
	//auto Reader = TJsonReaderFactory<>::Create(AbnormalInfo);
	//TSharedPtr<FJsonObject> Json;
	//if (FJsonSerializer::Deserialize(Reader, Json)) {
	//	auto AbnormalInfos = Json->GetArrayField(L"AbnormalInfos");

	//	for (auto& Info : AbnormalInfos)
	//	{
	//		//x 20200103 查问题 Broadcast是一个单词，cast的c是小写。多播委托是DECLARE_DYNAMIC_MULTICAST_DELEGATE_，DYNAMIC单词拼错了...
	//		auto i = Info->AsObject();
	//		if (i.IsValid()) {
	//			auto id = i->GetIntegerField(L"id");
	//			auto type = i->GetStringField(L"type");
	//			auto pos = i->GetStringField(L"pos");
	//			TArray<FString> _;
	//			pos.ParseIntoArray(_, L" ");
	//			FTransform TargetTransform;
	//			if (_.Num() >= 3)
	//			{
	//				auto Translation = FVector(FCString::Atof(*_[0]), FCString::Atof(*_[1]), FCString::Atof(*_[2]));
	//				TargetTransform.SetLocation(Translation);
	//			}
	//			if (_.Num() >= 6)
	//			{
	//				auto Rotation = FRotator(FCString::Atof(*_[4]), FCString::Atof(*_[5]), FCString::Atof(*_[3]));
	//				TargetTransform.SetRotation(Rotation.Quaternion());
	//			}
	//			if (_.Num() >= 9)
	//			{
	//				auto Scale = FVector(FCString::Atof(*_[6]), FCString::Atof(*_[7]), FCString::Atof(*_[8]));
	//				TargetTransform.SetScale3D(Scale);
	//			}
	//			auto Tuple = MakeTuple(type, TargetTransform);
	//			TempInfos.Add(id, Tuple);
	//			OnAbnormalInfoChange.Broadcast(id, type, TargetTransform);
	//		}
	//	}
	//	//DealWithOneAbnormalTask(TempInfos);
	//}
}

#if WITH_EDITOR
void AAbnormalManager::PostEditChangeProperty(struct FPropertyChangedEvent &PropertyChangedEvent)
{
	auto PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AAbnormalManager, Abnormals))
	{
		Abnormals.KeySort([](const FString &A, const FString &B) {
			return A < B;
		});
		// UE_LOG(LogAbnormalPlugin, Error, TEXT("Trigger Property Changed"));
	}
}
#endif // WITH_EDITOR

// Called when the game starts or when spawned
void AAbnormalManager::BeginPlay()
{
	// 运行时先整合分布在不同关卡中的非正常任务信息，这样可以解决具体非正常想挂载到具体目标点上的问题
	// FIXME 如果是关卡流加载形式，BeginPlay中执行一次是否能获取所有填表信息？这里有待检验的逻辑漏洞
	// 验证了一半，关卡流被加载进来的已经都能获取到了，目前并不会测试没有被加载的
	IntegrateAllAbnormalTaskInfoToAbnormals(OUT Abnormals);
	Super::BeginPlay();
}

// Called every frame
void AAbnormalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Sets default values
AAbnormalManager::AAbnormalManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}