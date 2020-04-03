// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalBase.generated.h"

UCLASS()
class ABNORMALPLUGIN_API AAbnormalBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbnormalBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abnormal | Base")
	float PlayRate = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abnormal | Base")
	float DelaySeconds = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abnormal | Base")
	bool bHidePreviewWindow = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abnormal | Base")
	bool bOnTrainAbnormal = false;

	bool bDynamicGenerated = true;

	UPROPERTY(BlueprintReadWrite, Category = "Abnormal | Base")
	class UAbnormalPopMenu* AbnormalPopMenu;
	

	/// 播放Actor中ActorSequenceComponent组件包含的动画
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Abnormal | Base")
	void StartPlaySequence();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Abnormal | Base")
	/**
	 * @brief 在播放动画前的设置。执行流程：
	 * 绑定非正常Actor到TargetActors --> 预设置 --> 播放序列
	 */
	void PreSet();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abnormal | Base")
	/**
	 * @brief 注册交互式菜单，第一版为弹出式菜单
	 * 
	 */
	void RegisterMenu();
	void RegisterMenu_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abnormal | Base")
	/**
	 * @brief 触发处理流程，弹出菜单中的默认处理按钮的点击事件中会调用该函数
	 * 
	 */
	void Process();
	void Process_Implementation();
	
	UFUNCTION(BlueprintCallable, Category = "Abnormal | Base")
	/**
	 * @brief 获取该Actor的非正常Id，用于删除/隐藏用
	 * 
	 * @return const FString& 非正常任务Id
	 */
	const FString& GetAbnormalId();

	UFUNCTION(BlueprintCallable, Category = "Abnormal | Base")
	/**
	 * @brief 设置非正常Actor属于哪一条非正常任务
	 * 
	 * @param TaskId 外部发来的Id
	 */
	void SetAbnormalId(const FString& TaskId);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Abnormal | Base")
	bool CustomAttachToTargetActor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Abnormal | Base")
	bool RigisterPreviewWindow();
private:
	FString AbnormalId;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
