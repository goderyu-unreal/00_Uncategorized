// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalBase.h"
#include "AbnormalManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBindingActorFinished);

USTRUCT(BlueprintType)
struct FAbnormalInfo
{
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
	/// 欲挂载的目标点，一个单位的非正常动画可以挂载到多个目标点上
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
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
class ABNORMALPLUGIN_API AAbnormalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbnormalManager();

	/// 非正常任务设置，Key填写非正常任务名，要确保和外部发送的非正常任务名保持一致
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	TMap<FString, FAbnormalsInfo> Abnormals;

	UPROPERTY(BlueprintAssignable, Category = Abnomal)
	FOnBindingActorFinished OnBindingActorFinished;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Abnormal, meta = (WorldContext = "WorldContextObject", DisplayName = "Get Abnormal Manager"))
	static AAbnormalManager* GetInstance(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Abnormal)
	/**
	 * @brief 开始执行任务
	 * 
	 * @param AbnormalId 任务ID
	 * @param AbnormalTaskName 任务名，用来和Abnormals中的键对应
	 * @param TargetTransforms 位置信息，用来生成TargetPoint
	 */
	void TriggerTask(const FString& AbnormalId, const FString& AbnormalTaskName, const TArray<FTransform>& TargetTransforms);

	void TriggerTask_Implementation(const FString& AbnormalId, const FString& AbnormalTaskName, const TArray<FTransform>& TargetTransforms);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Abnormal)
	/**
	 * @brief 根据TargetActors的个数生成AbnormalClass指定类的Actor
	 * 生成的Actor依次附加到TargetActor上
	 * 
	 * @param AbnormalId 任务ID，生成Actor时添加Tag，用于删除Actor时索引 
	 * @param AbnormalClass 要生成Actor的类
	 * @param TargetActors 要附加的Actor数组
	 */
	void SpawnAndBindingAbnormalActorToTargetActors(const FString& AbnormalId, UClass* AbnormalClass, const TArray<class AActor*>& TargetActors);

	void SpawnAndBindingAbnormalActorToTargetActors_Implementation(const FString& AbnormalId, UClass* AbnormalClass, const TArray<class AActor*>& TargetActors);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Abnormal)
	/**
	 * @brief 根据TargetActors的个数生成AbnormalClass指定类的Actor
	 * 生成的Actor依次附加到TargetActor上
	 * 
	 * @param AbnormalId 任务ID，生成Actor时添加Tag，用于删除Actor时索引 
	 * @param AbnormalClass 要生成Actor的类
	 * @param TargetActors 要附加的Actor数组
	 */
	void BindingAbnormalActorToTargetActors(const FString& AbnormalId, AAbnormalBase* AbnormalActor, AActor* TargetActor);

	void BindingAbnormalActorToTargetActors_Implementation(const FString& AbnormalId, AAbnormalBase* AbnormalActor, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Abnormal)
	/**
	 * @brief 根据TargetTransforms在每一个Transform生成一个ATargetPoint
	 * 并为每一个Actor设置值为AbnormalId的Tag
	 * 
	 * @param AbnormalId 
	 * @param TargetTransforms 
	 * @return const TArray<class AActor*> 生成的Actor数组
	 */
	const TArray<class AActor*> SpawnTargetActor(const FString& AbnormalId, const TArray<FTransform>& TargetTransforms);

	const TArray<class AActor*> SpawnTargetActor_Implementation(const FString& AbnormalId, const TArray<FTransform>& TargetTransforms);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Abnormal)
	/**
	 * @brief 根据IdTag销毁动态生成的TargetActor和AbnormalActor
	 * 
	 * @param AbnormalId 
	 * @return true 如果每一个带有AbnormalId标签的Actor销毁成功则返回
	 * @return false 只要有一个没有销毁成功返回false
	 */
	bool DestroyAbnormalActorsById(const FString& AbnormalId);

	bool DestroyAbnormalActorsById_Implementation(const FString& AbnormalId);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};