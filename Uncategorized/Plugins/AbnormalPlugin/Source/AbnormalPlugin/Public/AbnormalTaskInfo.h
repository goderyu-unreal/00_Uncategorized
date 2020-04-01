// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalTaskInfo.generated.h"

UENUM(BlueprintType)
enum class ETargetTransformSource : uint8
{
	TTS_FromOutside      = 2 UMETA(DisplayName = "外部提供"),
	TTS_FromTargetActors = 4 UMETA(DisplayName = "目标指定"),
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
	/// 动态生成非正常Actor标识，从实例中选择和从类中选择只能二选一
	EAbnormalActorType AbnormalActorType = EAbnormalActorType::AAT_DynamicGenerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 16))
	/// 未拖入场景中的非正常Actor类，只能指定AbnormalBase类及其子类
	TSubclassOf<class AAbnormalBase> AbnormalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (AbnormalActorType = 32))
	/// 场景中已存在的非正常Actor，具体类型的非正常，如铁路维修人员施工非正常Actor、石头滚落Actor
	AAbnormalBase* AbnormalActor;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	/// 挂载点位置信息来源
	/// 自身提供：将由非正常Actor的具体填表属性来决定出现的位置。例如第一节车厢的中心偏移相对距离
	/// 目标指定：将由填表信息TargetActors来指定具体的挂载点。例如线路火灾直接使用场景中某段线路旁的目标点
	/// 外部提供：将由下发非正常任务的外部程序提供位置信息。例如外部发送落石非正常并发送了一个世界坐标
	ETargetTransformSource TargetTransformSource = ETargetTransformSource::TTS_FromOutside;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (TargetTransformSource = 4, AbnormalActorType = 16))
	/// 欲挂载的目标点，一个单位的非正常动画可以挂载到多个目标点上
	TArray<AActor*> TargetActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (TargetTransformSource = 4, AbnormalActorType = 32))
	/// 欲挂载的目标点，拖入关卡中的非正常Actor只能挂载在一个Actor上
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	/// 除了设定外部指定/关卡内指定/自定义的位置，再附加一个Transform量
	/// 例如本来设置的位置是(10,10,10)，旋转是(0,0,90)，缩放是(1,1,1)，
	/// 现在设置AdditiveTransform的值为(-5,-5,-5),(90,0,0),(2,2,2)，
	/// 则最终结果为(5,5,5),(90,0,90),(2,2,2)。
	/// 即平移和旋转是增量模式，缩放是设置（覆盖）模式
	FTransform AdditiveTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	/// 指定该非正常动画的播放速率
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	/// 设置延迟播放的时间
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
