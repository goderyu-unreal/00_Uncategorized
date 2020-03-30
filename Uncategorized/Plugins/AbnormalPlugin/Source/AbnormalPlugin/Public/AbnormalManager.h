// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalBase.h"
#include "AbnormalTaskInfo.h"
#include "AbnormalManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAddAbnormal, const FString&, AbnormalTaskId, const FString&, AbnormalTaskName, const FTransform&, TargetTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteAbnormal, const FString&, AbnormalTaskId);

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

	UPROPERTY(BlueprintAssignable, Category = Abnormal)
	FOnAddAbnormal OnAddAbnormal;

	UPROPERTY(BlueprintAssignable, Category = Abnormal)
	FOnDeleteAbnormal OnDeleteAbnormal;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Abnormal, meta = (WorldContext = "WorldContextObject", DisplayName = "Get Abnormal Manager"))
	static AAbnormalManager* GetInstance(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintNativeEvent, Category = Abnormal)
	void IntegrateAllAbnormalTaskInfoToAbnormals(TMap<FString, FAbnormalsInfo>& AbnormalsMap);

	void IntegrateAllAbnormalTaskInfoToAbnormals_Implementation(TMap<FString, FAbnormalsInfo>& AbnormalsMap);

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
	void BindingAbnormalActorToTargetActors(const FString& AbnormalId, AAbnormalBase* AbnormalActor, const TArray<AActor*>& TargetActors);

	void BindingAbnormalActorToTargetActors_Implementation(const FString& AbnormalId, AAbnormalBase* AbnormalActor, const TArray<AActor*>& TargetActors);

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


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 解析通道数据，解析出命令，非正常任务名等信息
	 * 
	 * @param AbnormalInfo 通道中的值
	 */
	void UpdateAbnormalInfoFromString(const FString& AbnormalInfo);
	void UpdateAbnormalInfoFromString_Implementation(const FString& AbnormalInfo);

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