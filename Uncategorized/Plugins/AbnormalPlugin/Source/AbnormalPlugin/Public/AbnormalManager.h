// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbnormalBase.h"
#include "AbnormalTaskInfo.h"
#include "ConfigFileManager.h"
#include "AbnormalManager.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAddAbnormal, const FString&, AbnormalTaskId, const FString&, AbnormalTaskName, const FTransform&, TargetTransform);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteAbnormal, const FString&, AbnormalTaskId);

UCLASS()
class ABNORMALPLUGIN_API AAbnormalManager : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	/// 非正常任务设置，Key填写非正常任务名，要确保和外部发送的非正常任务名保持一致
	TMap<FString, FAbnormalsInfo> Abnormals;

	// UPROPERTY(BlueprintAssignable, Category = Abnormal)
	// FOnAddAbnormal OnAddAbnormal;

	// UPROPERTY(BlueprintAssignable, Category = Abnormal)
	// FOnDeleteAbnormal OnDeleteAbnormal;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Abnormal, meta = (WorldContext = "WorldContextObject", DisplayName = "Get Abnormal Manager"))
	static AAbnormalManager* GetInstance(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintNativeEvent, Category = Abnormal)
	/**
	 * @brief 收集所有的AbnormalTaskInfo的填表信息，将数据存入AbnormalsMap中
	 * 
	 * @param AbnormalsMap 整合进入的Map
	 */
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
	void TriggerTask(const FString& AbnormalId, const FString& AbnormalTaskName, const FTransform& TargetTransform);
	void TriggerTask_Implementation(const FString& AbnormalId, const FString& AbnormalTaskName, const FTransform& TargetTransform);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 生成非正常Actor，为其追加值为AbnormalId的Tag
	 * 
	 * @param AbnormalInfo 填表信息
	 * @param AbnormalId 非正常任务Id
	 * @param AbnormalTaskName 非正常任务名，打印日志时使用
	 * @return true 注册成功
	 * @return false 注册出现问题
	 */
	bool RegisterAbnormalActor(FAbnormalInfo& AbnormalInfo, const FString& AbnormalId, const FString& AbnormalTaskName);
	inline bool RegisterAbnormalActor_Implementation(FAbnormalInfo& AbnormalInfo, const FString& AbnormalId, const FString& AbnormalTaskName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 在TargetTransform处生成Tag为AbnormalId的TargetActor
	 * 
	 * @param AbnormalInfo 填表信息中的单项
	 * @param AbnormalId 外部发来的非正常任务Id
	 * @param TargetTransform 变换信息
	 */

	/**
	 * @brief 在TargetTransform处生成Tag为AbnormalId的TargetActor
	 * 
	 * @param AbnormalInfo 填表信息中的单项
	 * @param AbnormalId 外部发来的非正常任务Id
	 * @param TargetTransform 变换信息
	 * @param AbnormalTaskName 非正常任务名，打印日志时使用
	 * @return true 注册成功
	 * @return false 注册出现问题
	 */
	bool RegisterTargetActor(FAbnormalInfo& AbnormalInfo, const FString& AbnormalId, const FTransform& TargetTransform, const FString& AbnormalTaskName);
	inline bool RegisterTargetActor_Implementation(FAbnormalInfo& AbnormalInfo, const FString& AbnormalId, const FTransform& TargetTransform, const FString& AbnormalTaskName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 将AbnormalActor附加到TargetActor上
	 * 
	 * @param AbnormalInfo 填表信息中的单项
	 */
	void AttachAbnormalToTarget(FAbnormalInfo& AbnormalInfo);
	inline void AttachAbnormalToTarget_Implementation(FAbnormalInfo& AbnormalInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 为AbnormalActor添加偏移变换
	 * 
	 * @param AbnormalInfo 填表信息
	 */
	void SetAdditiveTransform(FAbnormalInfo& AbnormalInfo);
	inline void SetAdditiveTransform_Implementation(FAbnormalInfo& AbnormalInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 对非正常Actor的各种操作，内部都是调用非正常Actor的一些接口
	 * 
	 * @param AbnormalInfo 填表信息
	 */
	void InitAbnormalState(FAbnormalInfo& AbnormalInfo);
	inline void InitAbnormalState_Implementation(FAbnormalInfo& AbnormalInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Abnormal)
	/**
	 * @brief 根据IdTag销毁动态生成的TargetActor和AbnormalActor
	 * 
	 * @param AbnormalId 
	 * @return true 如果每一个带有AbnormalId标签的Actor销毁成功则返回
	 * @return false 只要有一个没有销毁成功返回false
	 */
	bool DestroyAbnormalActorsById(const FString& AbnormalId);
	// TODO 为非正常Actor基类添加一个属性，是否是动态生成的标识，用于外部删除项点时隐藏指定实例
	bool DestroyAbnormalActorsById_Implementation(const FString& AbnormalId);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 解析通道数据，解析出命令，非正常任务名等信息
	 * 
	 * @param AbnormalInfo 通道中的值
	 */
	void UpdateAbnormalInfoFromString(const FString& AbnormalInfo);
	void UpdateAbnormalInfoFromString_Implementation(const FString& AbnormalInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	void AnalysisAbnormalPreviewInfo(const FString& AbnormalPreviewInfo);
	void AnalysisAbnormalPreviewInfo_Implementation(const FString& AbnormalPreviewInfo);
public:

	// Sets default values for this actor's properties
	AAbnormalManager();

	/**
	 * @brief 根据TargetTransform生成一个ATargetPoint对象
	 * 为其设置值为AbnormalId的Tag
	 * 
	 * @param AbnormalId 
	 * @param TargetTransform 
	 * @param SpawnParameters 
	 * @return AActor* 
	 */
	AActor* SpawnTargetActor(const FString& AbnormalId, const FTransform& TargetTransform, const FActorSpawnParameters& SpawnParameters) const;

	/**
	 * @brief 根据指定的非正常类生成Actor，位置为默认的Transform
	 * 为其设置值为AbnormalId的Tag
	 * 
	 * @param AbnormalId 
	 * @param AbnormalClass 
	 * @param SpawnParameters 
	 * @return AAbnormalBase* 
	 */
	AAbnormalBase* SpawnAbnormalActor(const FString& AbnormalId, UClass* AbnormalClass, const FActorSpawnParameters& SpawnParameters) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UConfigFileManager ConfigFileManager;

	AActor* PreviewCameraActor = nullptr;
};