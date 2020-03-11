// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalTaskInfo.generated.h"

USTRUCT(BlueprintType)
struct FAbnormalInfo
{
	// TODO 重构填表结构，将bDynamicInputTransform控制的两种状态改为由枚举控制的多种状态
	GENERATED_USTRUCT_BODY()
	/// 动态生成非正常Actor标识，从实例中选择和从类中选择只能二选一
	UPROPERTY(EditAnywhere, Category = Abnormal, meta = (InlineEditConditionToggle))
	bool bDynamicGenerateActor = true;
	/// 场景中已存在的非正常Actor，具体类型的非正常，如铁路维修人员施工非正常Actor、石头滚落Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "!bDynamicGenerateActor"))
	class AAbnormalBase* AbnormalActor;
	/// 未拖入场景中的非正常Actor类，只能指定AbnormalBase类及其子类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "bDynamicGenerateActor"))
	TSubclassOf<AAbnormalBase> AbnormalClass;
	/// 是否采用外部提供的位置数据，如果不需要，
	UPROPERTY(EditAnywhere, Category = Abnormal, meta = (InlineEditConditionToggle))
	bool bDynamicInputTransform = false;
	/// 欲挂载的目标点，一个单位的非正常动画可以挂载到多个目标点上
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "!bDynamicInputTransform"))
	TArray<AActor*> TargetActors;

	/// 默认播放一次，设置为0或负数表示无限循环播放
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, AdvancedDisplay)
	int32 PlayCount = 1;
	/// 设置延迟播放的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, AdvancedDisplay)
	float DelaySeconds;
};

USTRUCT(BlueprintType)
struct FAbnormalsInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)	
	TArray<FAbnormalInfo> AbnormalsInfo;
};

UCLASS()
class ABNORMALPLUGIN_API AAbnormalTaskInfo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbnormalTaskInfo();

	/// 非正常任务设置，Key填写非正常任务名，要确保和外部发送的非正常任务名保持一致
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	TMap<FString, FAbnormalsInfo> ExtraAbnormals;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
