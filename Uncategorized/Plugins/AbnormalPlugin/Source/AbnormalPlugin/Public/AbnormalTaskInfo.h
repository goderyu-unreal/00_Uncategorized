// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalTaskInfo.generated.h"

UENUM(BlueprintType)
enum class ETargetTransformSource : uint8
{
	TTS_FROMSELF         UMETA(DisplayName = "自身提供"),
	TTS_FROMTARGETACTORS UMETA(DisplayName = "目标指定"),
	TTS_FROMOUTSIDE      UMETA(DisplayName = "外部提供")
};


USTRUCT(BlueprintType)
struct FAbnormalInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category = Abnormal, meta = (InlineEditConditionToggle))
	/// 动态生成非正常Actor标识，从实例中选择和从类中选择只能二选一
	bool bDynamicGenerateActor = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "!bDynamicGenerateActor"))
	/// 场景中已存在的非正常Actor，具体类型的非正常，如铁路维修人员施工非正常Actor、石头滚落Actor
	class AAbnormalBase* AbnormalActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "bDynamicGenerateActor"))
	/// 未拖入场景中的非正常Actor类，只能指定AbnormalBase类及其子类
	TSubclassOf<AAbnormalBase> AbnormalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	/// 挂载点位置信息来源
	/// 自身提供：将由非正常Actor的具体填表属性来决定出现的位置。例如第一节车厢的中心偏移相对距离
	/// 目标指定：将由填表信息TargetActors来指定具体的挂载点。例如线路火灾直接使用场景中某段线路旁的目标点
	/// 外部提供：将由下发非正常任务的外部程序提供位置信息。例如外部发送落石非正常并发送了一个世界坐标
	ETargetTransformSource TargetTransformSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "TargetTransformSource == ETargetTransformSource::TTS_FROMTARGETACTORS"))
	/// 欲挂载的目标点，一个单位的非正常动画可以挂载到多个目标点上
	TArray<AActor*> TargetActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, AdvancedDisplay)
	/// 增量偏移(除了设定外部指定/关卡内指定/自定义的位置，再附加一个Transform增量)
	/// 例如本来设置的位置是(10,10,10)，旋转是(0,0,90)，缩放是(1,1,1)，
	/// 现在设置AdditiveTransform的值为(-5,-5,-5),(90,0,0),(1,1,1)，
	/// 则最终结果为(5,5,5),(90,0,90),(2,2,2)
	FTransform AdditiveTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, AdvancedDisplay)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, AdvancedDisplay)
	/// 设置延迟播放的时间
	float DelaySeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, AdvancedDisplay)
	/// 默认播放一次，设置为0或负数表示无限循环播放
	int32 PlayCount = 1;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	/// 非正常任务设置，Key填写非正常任务名，要确保和外部发送的非正常任务名保持一致
	TMap<FString, FAbnormalsInfo> ExtraAbnormals;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
