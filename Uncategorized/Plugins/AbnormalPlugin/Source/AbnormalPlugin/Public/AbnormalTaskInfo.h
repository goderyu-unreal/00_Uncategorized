// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbnormalTaskInfo.generated.h"

UENUM(BlueprintType)
enum class ETargetTransformSource : uint8
{
	TTS_FromOutside      = 2 UMETA(DisplayName = "外部提供"),
	TTS_FromTargetActor  = 4 UMETA(DisplayName = "目标指定"),
	TTS_FromSelf         = 8 UMETA(DisplayName = "自身提供"),
	TTS_None			 = 0 UMETA(DisplayName = "无")
};

UENUM(BlueprintType)
enum class EAbnormalActorType : uint8
{
	AAT_DynamicGenerate = 16 UMETA(DisplayName = "动态生成"),
	AAT_ExistedInstance = 32 UMETA(DisplayName = "指定实例"),
	AAT_None			= 0  UMETA(DisplayName = "无")
};

USTRUCT(BlueprintType)
struct FAbnormalInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category = Abnormal)
	/// 非正常Actor类型（蓝图类/关卡实例）
	/// 动态生成需要指定非正常蓝图类
	/// 指定实例需要指定当前关卡中放置的非正常Actor
	EAbnormalActorType AbnormalActorType = EAbnormalActorType::AAT_DynamicGenerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 16))
	/// 非正常蓝图类
	TSubclassOf<class AAbnormalBase> AbnormalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 32))
	/// 非正常Actor关卡实例
	AAbnormalBase* AbnormalActor;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 48))
	/// 挂载点位置信息来源
	/// 自身提供：将由非正常蓝图类的自定义函数CustomAttachToTargetActor来实现挂载操作
	/// 目标指定：将由填表信息TargetActor来指定具体的挂载点。例如线路火灾直接使用场景中某段线路旁的目标点
	/// 外部提供：将由下发非正常任务的外部程序提供位置信息。例如外部发送落石非正常并发送了一个线路世界坐标
	ETargetTransformSource TargetTransformSource = ETargetTransformSource::TTS_FromOutside;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 48, TargetTransformSource = 4))
	/// 挂载点，可以选择关卡中任意的Actor作为非正常Actor的父级对象
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 48))
	/// 除了设定外部指定/关卡内指定/自定义的位置，再附加一个Transform量
	/// 目前使用的是SetActorRelativeTransform的方式
	FTransform AdditiveTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 48))
	/// 设置AdditiveTransform变量的设置方式
	/// 勾选为使用AddActorWorldTransform方式
	/// 否则使用AddActorLocalTransform的方式
	uint8 bWorldTransform : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 48))
	/// 指定该非正常Actor里面的动画资源播放时的速率
	float PlayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 48))
	/// 设置在外部向通道写值要执行非正常任务后，非正常动画延迟多少秒开始播放
	float DelaySeconds = 0.f;
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

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
