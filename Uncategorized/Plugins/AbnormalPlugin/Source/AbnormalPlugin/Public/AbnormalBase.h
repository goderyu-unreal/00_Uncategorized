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

	/// 播放Actor中ActorSequenceComponent组件包含的动画
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Abnormal)
	void StartPlaySequence();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 在播放动画前的设置。执行流程：
	 * 绑定非正常Actor到TargetActors --> 预设置 --> 播放序列
	 */
	void PreSet();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 注册交互式菜单，第一版为弹出式菜单
	 * 
	 */
	void RegisterMenu();
	void RegisterMenu_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Abnormal)
	/**
	 * @brief 触发处理流程，弹出菜单中的默认处理按钮的点击事件中会调用该函数
	 * 
	 */
	void Process();
	void Process_Implementation();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
