// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalActor.h"
#include "ManagerAbnormal.generated.h"

USTRUCT(BlueprintType)
struct FAbnormalBindingss
{
	GENERATED_USTRUCT_BODY()
	/// 动态生成非正常Actor标识，从实例中选择和从类中选择只能二选一
	UPROPERTY(EditAnywhere, Category = Abnormal, meta = (InlineEditConditionToggle))
	bool bDynamicGenerateActor = true;
	/// 场景中已存在的非正常Actor，具体类型的非正常，如铁路维修人员施工非正常Actor、石头滚落Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "!bDynamicGenerateActor"))
	class AAbnormalActor* AbnormalActor;
	/// 未拖入场景中的非正常Actor类，只能指定AbnormalActor类及其子类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "bDynamicGenerateActor"))
	TSubclassOf<AAbnormalActor> AbnormalClass;
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
struct FAbnormalBindingsss
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)	
	TArray<FAbnormalBindingss> Abnormals;
};

USTRUCT(BlueprintType)
struct FAbnormalBindings
{
	GENERATED_USTRUCT_BODY()
	/// 此条非正常的唯一标识
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	FString Name;
	/// 动态生成非正常Actor标识，从实例中选择和从类中选择只能二选一
	UPROPERTY(EditAnywhere, Category = Abnormal, meta = (InlineEditConditionToggle))
	bool bDynamicGenerateActor = true;
	/// 场景中已存在的非正常Actor，具体类型的非正常，如铁路维修人员施工非正常Actor、石头滚落Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "!bDynamicGenerateActor"))
	class AAbnormalActor* AbnormalActor;
	/// 未拖入场景中的非正常Actor类，只能指定AbnormalActor类及其子类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal, meta = (EditCondition = "bDynamicGenerateActor", AllowedClasses = "AAbnormalActor"))
	class UClass* AbnormalClass;
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

UCLASS()
class ABNORMALPLUGIN_API AManagerAbnormal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManagerAbnormal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	TArray<FAbnormalBindings> Abnormals;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	TMap<FString, FAbnormalBindingss> Abnormalss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abnormal)
	TMap<FString, FAbnormalBindingsss> Abnormalsss;

	UFUNCTION(BlueprintCallable, Category = Abnormal)
	void ExecuteTrigger(const TArray<FString>&  AbnormalTaskNames);
	UFUNCTION(BlueprintCallable, Category = Abnormal)
	void ExecuteTrigger1(const TArray<FString>&  AbnormalTaskNames);
	UFUNCTION(BlueprintCallable, Category = Abnormal)
	void ExecuteTrigger2(const TArray<FString>&  AbnormalTaskNames);

	UFUNCTION(BlueprintNativeEvent, Category = Abnormal)
	void ExecuteBinding(AAbnormalActor* BindingActor, const TArray<class AActor*>& TargetActors);

	void ExecuteBinding_Implementation(AAbnormalActor* BindingActor, const TArray<class AActor*>& TargetActors);

	UFUNCTION(BlueprintNativeEvent, Category = Abnormal)
	void ExecuteSpawnAndBinding(UClass* AbnormalClass, const TArray<class AActor*>& TargetActors);

	void ExecuteSpawnAndBinding_Implementation(UClass* AbnormalClass, const TArray<class AActor*>& TargetActors);

	void ExecutePlay(AAbnormalActor* BindingActor);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
